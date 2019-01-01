/****************************************************************/
/*                                                              */
/*  Copyright (C) 2018, M. Andelkovic, L. Covaci, A. Ferreira,  */
/*                    S. M. Joao, J. V. Lopes, T. G. Rappoport  */
/*                                                              */
/****************************************************************/

#include <iomanip>
template <typename T>
class KPM_Vector <T, 2> : public KPM_VectorBasis <T,2> {
private:
  LatticeStructure<2u>       & r;
  std::size_t 	*MemIndBeg[2][2];
  std::size_t 	*MemIndEnd[2][2];
  std::size_t        block[2][2];
  std::size_t          stride[2];
  std::size_t   stride_ghosts[2];
  std::size_t      transf_max[2]; // [d][edged]
  std::size_t transf_bound[2][2]; // [d][edged]
  Hamiltonian<T,2u>          & h;
  T               ***mult_t1_ghost_cor;
  Coordinates<std::size_t,3>   x;
  T                        *phi0;
  T                       *phiM1;
  T                       *phiM2;
  const std::size_t          std;
  const unsigned D = 2u;
public:
  typedef typename extract_value_type<T>::value_type value_type;
  using KPM_VectorBasis<T,2>::simul;
  using KPM_VectorBasis<T,2>::index;
  using KPM_VectorBasis<T,2>::v;
  using KPM_VectorBasis<T,2>::memory;
  using KPM_VectorBasis<T,2>::aux_wr;
  using KPM_VectorBasis<T,2>::aux_test;
  using KPM_VectorBasis<T,2>::inc_index;
  
  KPM_Vector(int mem, Simulation<T,2> & sim) :
    KPM_VectorBasis<T,2>(mem, sim),  r(sim.r), stride{r.Ld[0], 1}, stride_ghosts{1,r.Ld[0]},
    transf_max{r.ld[1], r.Ld[0]}, h(sim.h), x(r.Ld), std(x.basis[1]) {
    unsigned d;
    Coordinates <std::size_t, 3>     z(r.Ld);
    Coordinates <int, 3> x(r.nd), dist(r.nd);
    
    mult_t1_ghost_cor = new T**[r.Orb];
    for(unsigned io = 0; io < r.Orb; io++)
      {
	mult_t1_ghost_cor[io] = new T*[h.hr.NHoppings(io)];
	for(unsigned ib = 0; ib < h.hr.NHoppings(io); ib++)
	  mult_t1_ghost_cor[io][ib] = new T[STRIDE];
      }

    for(unsigned d = 0; d < 2; d++)
      for(unsigned b = 0; b < 2; b++)
	{
	  MemIndBeg[d][b] = new std::size_t[r.Orb];
	  MemIndEnd[d][b] = new std::size_t[r.Orb];
	}
    
    
    for(unsigned d = 0; d < 2u; d++)
      for(unsigned edge = 0; edge < 2; edge++)
	transf_bound[d][edge] = (r.boundary[d][edge] == true ? r.ld[D - 1 - d] : 0);
    
    for(unsigned edge = 0 ; edge < 2; edge++)
      if(r.boundary[1][edge] == 1) // Botton/Top edge Increase the size due left and right corners
	transf_bound[1][edge] += (r.boundary[0][0] == 1 ? NGHOSTS : 0 ) + (r.boundary[0][1] == 1 ? NGHOSTS : 0 ) ;
    
    
    for(std::size_t io = 0; io < r.Orb; io++)
      {
	d = 0;
	// Position of initial corner to copy the source
	MemIndBeg[d][0][io] = z.set({std::size_t(NGHOSTS),               std::size_t(NGHOSTS), io}).index;   
	MemIndBeg[d][1][io] = z.set({std::size_t(r.Ld[0] - 2 * NGHOSTS), std::size_t(NGHOSTS), io}).index;   
	// Position of initial corner to copy to the destiny 
	MemIndEnd[d][0][io] = z.set({std::size_t(0),                     std::size_t(NGHOSTS), io}).index;   
	MemIndEnd[d][1][io] = z.set({std::size_t(r.Ld[0] - NGHOSTS),     std::size_t(NGHOSTS), io}).index;   
	
	d = 1;
	// Bottom edge 
	MemIndBeg[d][0][io] = z.set({std::size_t(NGHOSTS), std::size_t(NGHOSTS),             io}).index;
	MemIndEnd[d][0][io] = z.set({std::size_t(NGHOSTS), std::size_t(0),                   io}).index;          
	if(r.boundary[1][0] == 1 && r.boundary[0][0] == 1) // Add the Left bottom Corner
	  {
	    MemIndBeg[1][0][io] = z.set({std::size_t(0), std::size_t(NGHOSTS),             io}).index;
	    MemIndEnd[1][0][io] = z.set({std::size_t(0), std::size_t(0),                   io}).index;
	  }
	// Top Edge 
	MemIndBeg[d][1][io] = z.set({std::size_t(NGHOSTS), std::size_t(r.Ld[1] - 2*NGHOSTS), io}).index;
	MemIndEnd[d][1][io] = z.set({std::size_t(NGHOSTS), std::size_t(r.Ld[1] - NGHOSTS),   io}).index;
	if(r.boundary[1][1] == 1 && r.boundary[0][0] == 1) // Add the Left bottom Corner
	  {
	    MemIndBeg[1][1][io] = z.set({std::size_t(0), std::size_t(r.Ld[1] - 2*NGHOSTS), io}).index;
	    MemIndEnd[1][1][io] = z.set({std::size_t(0), std::size_t(r.Ld[1] - NGHOSTS),   io}).index;
	  }
      }
    
    
    for(d = 0 ; d < D; d++)
      for(unsigned b  = 0 ; b < 2; b++)
	{
	  dist.set({0,0,0});
	  dist.coord[d] = int(b) * 2 - 1;
	  block[d][b] = x.set_coord( int(r.thread_id) ).add(dist).index;
	}
    initiate_vector();
  };
  
  
  ~KPM_Vector(void){
    for(unsigned d = 0; d < 2; d++)
      for(unsigned b = 0; b < 2; b++)
	{
	  delete MemIndBeg[d][b];
	  delete MemIndEnd[d][b];
	    
	}
    for(unsigned io = 0; io < r.Orb;io++)
      {	
	for(unsigned ib = 0; ib < h.hr.NHoppings(io); ib++)
	  delete mult_t1_ghost_cor[io][ib];
	delete mult_t1_ghost_cor[io];
      }
    delete mult_t1_ghost_cor;
  }
  
  void initiate_vector() {
    index = 0;
    Coordinates<std::size_t, 3> x(r.Ld);
    for(std::size_t io = 0; io < r.Orb; io++)
      for(std::size_t i1 = NGHOSTS; i1 < r.Ld[1] - NGHOSTS; i1++)
	for(std::size_t i0 = NGHOSTS; i0 < r.Ld[0] - NGHOSTS; i0++)
	  v(x.set({i0,i1,io}).index, index) = simul.rnd.init()/static_cast<value_type>(sqrt(value_type(r.Sizet - r.SizetVacancies)));
    
    for(unsigned i = 0; i < r.NStr; i++)
      {
	auto & vv = h.hV.position.at(i); 
	for(unsigned j = 0; j < vv.size(); j++)
	  v(vv.at(j), index ) = 0. ;
      }
    
  };
  
  T get_point()
  {
    T point;
    if(r.thread_id == 0)
      {
	Coordinates<std::size_t, 3> x(r.Ld);
	std::size_t indice = x.set({std::size_t(r.Ld[0]/2),std::size_t(r.Ld[1]/2), std::size_t(0)}).index;
	point = v(indice,index);
      }
    else
      point = assign_value<T>(0,0);
    return point;
  }

  void build_wave_packet(Eigen::Matrix<double,-1,-1> & k, Eigen::Matrix<T,-1,-1> & psi0, double & sigma)
  {
    index = 0;
    Coordinates<std::size_t, 3> x(r.Ld), z(r.Lt);
    Eigen::Matrix<T,-1,-1>  sum(r.Orb, 1);
    Eigen::Map<Eigen::Matrix<std::size_t,2, 1>> vv(z.coord);
    Eigen::Matrix<double, 1, 2> va , vb;
    Eigen::Matrix<double, -1,-1> phase(1, psi0.cols());
    T soma = assign_value<T> (0,0);
    auto bbs = r.rLat.inverse(); // each row is a bi / 2*M_PI 
    auto vOrb = bbs * r.rOrb;    // each columns is a vector in a1 basis
    

    vb(0,0) = r.Lt[0]/2;
    vb(0,1) = r.Lt[1]/2;
    
    double a00 = r.rLat.col(0).transpose()*r.rLat.col(0);
    double a11 = r.rLat.col(1).transpose()*r.rLat.col(1);
    double a01 = r.rLat.col(0).transpose()*r.rLat.col(1);
    
#pragma omp master 
    {
      simul.Global.mu = Eigen::Matrix<T,-1,-1>(psi0.cols(), 1);      
      for(int ik = 0; ik < psi0.cols(); ik++)
	simul.Global.mu(ik, 0) = assign_value<T>( simul.rnd.get(), 0 );
    }
#pragma omp barrier

#pragma omp critical
    for(int ik = 0; ik < psi0.cols(); ik++)
      phase(0,ik)  = std::real(simul.Global.mu(ik, 0)) ;
#pragma omp barrier
    for(std::size_t i1 = NGHOSTS; i1 < r.Ld[1] - NGHOSTS; i1++)
      for(std::size_t i0 = NGHOSTS; i0 < r.Ld[0] - NGHOSTS; i0++)
	{
	  x.set({i0,i1,std::size_t(0)});
	  r.convertCoordinates(z,x);
	  double n1 = (double(z.coord[1]) - double(r.Lt[1])/2.)/sigma;
	  double n0 = (double(z.coord[0]) - double(r.Lt[0])/2.)/sigma;
	  double gauss = n0*n0 * a00 + n1*n1 * a11 + 2*n0*n1*a01;
	  sum.setZero();
	  va = vv.cast<double>().transpose();
	  
	  for(int ik = 0; ik < psi0.cols(); ik++)
	    {
	      for(unsigned io = 0; io < r.Orb; io++)
		{
		  double xx = (va  + vOrb.col(io).transpose() ) * k.col(ik);
		  sum(io, 0) += psi0(io,ik) * exp(assign_value<T>(-0.5*gauss,  2.*M_PI * (xx + 0.*phase(0,ik) )));
		}
	    }
	  
	  for(std::size_t io = 0; io < r.Orb; io++)
	    {
	      x.set({i0,i1,io});
	      v(x.index, 0) = sum(io,0);
	      soma += assign_value<T> ( std::real( sum(io,0) * std::conj(sum(io,0)) ), 0);
	    }
	}
    
#pragma omp master
    simul.Global.soma = assign_value<T> (0,0);
#pragma omp barrier
#pragma omp critical
    simul.Global.soma += soma;
#pragma omp barrier
    soma = simul.Global.soma;
    value_type soma2 = sqrt(std::real(soma));
    
    for(std::size_t io = 0; io < r.Orb; io++)
      for(std::size_t i1 = NGHOSTS; i1 < r.Ld[1] - NGHOSTS; i1++)
	for(std::size_t i0 = NGHOSTS; i0 < r.Ld[0] - NGHOSTS; i0++)
	  {
	    x.set({i0,i1,io});
	    v(x.index, 0) /= soma2;
	  }
  }
  
  
  template < unsigned MULT,bool VELOCITY> 
  void build_regular_phases(int i1, unsigned axis)
  {
    unsigned l[2 + 1], count;
    Coordinates<std::ptrdiff_t, 3>  global(r.Lt);
    Coordinates<std::ptrdiff_t, 3> local1(r.Ld);
    Coordinates<std::size_t,3> x(r.Ld);
    std::fill_n(l, 2, 3);
    l[2]  = r.Orb;
    Coordinates<std::ptrdiff_t, 2 + 1> b3(l);    
    Eigen::Map<Eigen::Matrix<std::ptrdiff_t,2, 1>> vee(b3.coord); // Column vector
    
    
    for(unsigned io = 0; io < r.Orb; io++)
      {
	const std::size_t ip = io * x.basis[2] ;
	const std::size_t j0 = ip + 0 + i1 * std;
	const std::size_t j1 = j0 + STRIDE * std;

	for(unsigned ib = 0; ib < h.hr.NHoppings(io); ib++)
	  {
	    T tt  = value_type(MULT + 1) * h.hr.hopping(ib, io);
	    b3.set_coord(h.hr.dist(ib,io));        
	    vee.array() -= 1;
	    count = 0;
	    if (VELOCITY)  tt  *=  h.hr.v.at(axis)(ib,io);
	    for(std::size_t j = j0; j < j1; j += std )
	      {
		r.convertCoordinates(global, local1.set_coord(j));
		value_type phase = vee(0)*global.coord[1]*r.ghost_pot(0,1);
		mult_t1_ghost_cor[io][ib][count] =  tt * h.ghosts_correlation(phase);
		count++;
	      }
	  }
      }
  }
    
  template < unsigned MULT> 
  void initiate_stride(std::size_t & istr)
  {
    std::size_t i0, i1;
    const std::size_t std = x.basis[1];
    // Periodic component of the Hamiltonian + Anderson disorder
    i0 = ((istr) % (r.lStr[0]) ) * STRIDE + NGHOSTS;
    i1 = ((istr) / r.lStr[0] ) * STRIDE + NGHOSTS;
			
    for(std::size_t io = 0; io < r.Orb; io++)
      {
	const std::size_t ip = io * x.basis[2];
	const std::size_t j0 = ip + i0 + i1 * std;
	const std::size_t j1 = j0 + STRIDE * std; //j0 and j1 define the limits of the for cycle


	for(std::size_t j = j0; j < j1; j += std )
	  for(std::size_t i = j; i < j + STRIDE ; i++)
	    phi0[i] = - value_type(MULT) * phiM2[i];
      }
  }
				
  template < unsigned MULT> 
  void inline mult_local_disorder(const  std::size_t & j0, const  std::size_t & io)
  {
    const std::size_t j1 = j0 + STRIDE * std;
    const std::ptrdiff_t dd = (h.Anderson_orb_address[io] - std::ptrdiff_t(io))*r.Nd;
    // Anderson disorder
    if( h.Anderson_orb_address[io] >= 0)
      {
	for(std::size_t j = j0; j < j1; j += std )
	  for(std::size_t i = j; i < j + STRIDE ; i++)
	    phi0[i] += value_type(MULT + 1) * phiM1[i] * h.U_Anderson.at(i + dd);
      }
    else if (h.Anderson_orb_address[io] == - 1)
      {
	for(std::size_t j = j0; j < j1; j += std )
	  for(std::size_t i = j; i < j + STRIDE ; i++)
	    phi0[i] += value_type(MULT + 1) * phiM1[i] * h.U_Orbital.at(io);
      }
  }
	      
  void inline mult_regular_hoppings(const  std::size_t & j0, const  std::size_t & io)
  {
    std::size_t count;
    const std::size_t j1 = j0 + STRIDE * std;
    // Hoppings
    for(unsigned ib = 0; ib < h.hr.NHoppings(io); ib++)
      {
	const std::ptrdiff_t d1 = h.hr.distance(ib, io);
	count = 0;
	for(std::size_t j = j0; j < j1; j += std )
	  {
	    const T t1 = mult_t1_ghost_cor[io][ib][count++];
	    for(std::size_t i = j; i < j + STRIDE ; i++)
	      phi0[i] += t1 * phiM1[i + d1];								
	  }
      }
  }
			
			
  // Structural disorder contribution - iterate over the disorder models			
  template <unsigned MULT> 
  void Multiply() {
    vverbose_message("Entered Multiply");
    
    unsigned i = 0;
    /*
      Mosaic Multiplication using a TILE of STRIDE x STRIDE 
      Right Now We expect that both ld[0] and ld[1]  are multiple of STRIDE
      MULT = 0 : For the case of the Velocity/Hamiltonian
      MULT = 1 : For the case of the KPM_iteration
    */
    inc_index();
    phi0 = v.col(index).data();
    phiM1 = v.col((memory + index - 1) % memory ).data();
    phiM2 = v.col((memory + index - 2) % memory ).data();
    KPM_MOTOR<MULT, false>(phi0, phiM1, phiM2, i);
  };


  void Velocity(T * phi0,T * phiM1, unsigned axis) {
    KPM_MOTOR<0u, true>(phi0, phiM1, phiM1, axis);
  };
  
  template <unsigned MULT, bool VELOCITY>
  void KPM_MOTOR(T * phi0a, T * phiM1a, T *phiM2a, unsigned axis)
  {
    std::size_t i0, i1;    
    phi0 = phi0a;
    phiM1 = phiM1a;
    phiM2 = phiM2a;
    
    // Initialize tiles that have deffects connecting elements of a previous tile
    for(auto istr = h.cross_mozaic_indexes.begin(); istr != h.cross_mozaic_indexes.end() ; istr++)
      initiate_stride<MULT>(*istr);
    
    for( i1 = NGHOSTS; i1 < r.Ld[1] - NGHOSTS; i1 += STRIDE  )
      {
	build_regular_phases<MULT,VELOCITY>(i1, axis);
		
	for( i0 = NGHOSTS; i0 < r.Ld[0] - NGHOSTS; i0 += STRIDE )
	  {
		    
	    std::size_t istr = (i1 - NGHOSTS) /STRIDE * r.lStr[0] + (i0 - NGHOSTS)/ STRIDE;
	    if(h.cross_mozaic.at(istr))
	      initiate_stride<MULT>(istr);
	    // These four lines pertrain only to the magnetic field
	    for(std::size_t io = 0; io < r.Orb; io++)
	      {
		const std::size_t ip = io * x.basis[2];
		const std::size_t j0 = ip + i0 + i1 * std;
		
		// Local Energy
		if(!VELOCITY) mult_local_disorder<MULT>(j0, io);
		
		// Hoppings
		mult_regular_hoppings(j0, io);
	      }
	    for(auto id = h.hd.begin(); id != h.hd.end(); id++)
	      id->template multiply_defect<MULT, VELOCITY>(istr, phi0, phiM1, axis);
	  	    
	    // Empty the vacancies in the tile
	    auto & hV = h.hV.position.at(istr);
	    for(auto k = hV.begin(); k != hV.end(); k++)
	      phi0[*k] = 0.;

	  }
      }

    for(auto vc =  h.hV.vacancies_with_defects.begin(); vc != h.hV.vacancies_with_defects.end(); vc++)
      phi0[*vc] = 0.;

    
    /* 
       Broken Imputirities:
       The bulk domain will receive contributions from the broken defects 
       located on the neighbour domains.
       We already subtract the vacancies from these contributions 
    */
    for(auto id = h.hd.begin(); id != h.hd.end(); id++)
      id->template multiply_broken_defect<MULT,VELOCITY>(phi0, phiM1,axis);
	  
    // These four lines pertrain only to the magnetic field
    Exchange_Boundaries();
  };
  
  void measure_wave_packet(T * bra, T * ket, T * results)  
  {
    Coordinates<std::size_t,3> ad(r.Ld), at(r.Lt);
    ad.set({std::size_t(NGHOSTS), std::size_t(NGHOSTS), std::size_t(0)});
    r.convertCoordinates(at,ad);
    for(unsigned i = 0; i < 4; i++)
      results[i] *= 0.;

    for(unsigned io = 0; io < r.Orb; io++)
      {
	value_type deltax = r.rOrb(0,io);
	value_type deltay = r.rOrb(1,io);
	
	for(unsigned i1 = 0; i1 < r.ld[1]; i1++)
	  {
	    std::size_t ind = ad.set({std::size_t(NGHOSTS),std::size_t(i1 + NGHOSTS), std::size_t(io)}).index;
	    value_type z1 = at.coord[1] + i1;
	    value_type x0 = at.coord[0]*r.rLat(0,0) + z1 * r.rLat(0,1) + deltax;
	    value_type y0 = at.coord[0]*r.rLat(1,0) + z1 * r.rLat(1,1) + deltay;	    

	    T xl1 = assign_value<T>(0., 0.);
	    T xl2 = assign_value<T>(0., 0.);
	    T yl1 = assign_value<T>(0., 0.);
	    T yl2 = assign_value<T>(0., 0.);
	    
	    for(unsigned i0 = 0; i0 < r.ld[0]; i0++)
	      {
		std::size_t j0 = ind + i0;
		value_type x = x0 + i0 * r.rLat(0,0);
		value_type y = y0 + i0 * r.rLat(1,0);
		T p = myconj(*(bra + j0)) * (*(ket + j0));
		xl1 += p * x;
		xl2 += p * x * x;
		yl1 += p * y;
		yl2 += p * y * y;
      }
    
	    results[0] += xl1;
	    results[1] += xl2;
	    results[2] += yl1;
	    results[3] += yl2;
	  }
      }
	  // Periodic component of the Hamiltonian + Anderson disorder
  };
	  
  

  void Exchange_Boundaries() {
    /*
      I have four boundaries to exchange with the other threads.
      First I will copy the lines along the a[1] direction to a consecutive shared vector
    */
#pragma omp barrier    
    Coordinates<std::size_t,3u> x(r.Ld), z(r.Lt);
    T  *phi = v.col(index).data();

    
    for(unsigned d = 0; d < 2; d++)
      {
	std::size_t BSize = r.Orb * transf_max[d] * NGHOSTS;
	T * ghosts_left = & simul.ghosts[0];
	T * ghosts_right = & simul.ghosts[BSize];
#pragma omp critical
	{
	  std::cout << "dim : " << d << " thread : " << r.thread_id << std::endl;
	  std::cout << transf_bound[d][0] << std::endl;
	  std::cout << transf_bound[d][1] << std::endl;
	}
	for(std::size_t io = 0; io < r.Orb; io++)
	  {
	    std::size_t il = MemIndBeg[d][0][io];
	    std::size_t ir = MemIndBeg[d][1][io];
	    
	    for(std::size_t i = 0; i < transf_bound[d][0]; i++)
	      {
		for(unsigned ig = 0; ig < NGHOSTS; ig++)
		  ghosts_left [i + (ig + NGHOSTS*io) * transf_bound[d][0] ] = phi[il + ig*stride_ghosts[d]];
		il += stride[d];
	      }
	    
	    for(std::size_t i = 0; i < transf_bound[d][1]; i++)
	      {
		for(unsigned ig = 0; ig < NGHOSTS; ig++)
		  ghosts_right[i + (ig + NGHOSTS*io) * transf_bound[d][1] ] = phi[ir + ig*stride_ghosts[d]];
		ir += stride[d];
	      }
	  }
	
	// Copy the boundaries to the shared memory
	std::copy( ghosts_left, ghosts_left + 2*BSize, simul.Global.ghosts.begin() + 2*BSize * r.thread_id );	  
#pragma omp barrier
	auto neigh_left = simul.Global.ghosts.begin() + 2 * block[d][0] * BSize;
	auto neigh_right  = simul.Global.ghosts.begin() + 2 * block[d][1] * BSize;
	std::copy(neigh_right,         neigh_right + BSize , ghosts_right );     // From the left to the right
	std::copy(neigh_left + BSize,  neigh_left + 2*BSize, ghosts_left  )  ;   // From the right to the left
	
#pragma omp barrier	
	for(std::size_t io = 0; io < r.Orb; io++)
	  {
	    std::size_t il = MemIndEnd[d][0][io];
	    std::size_t ir = MemIndEnd[d][1][io];
	    
	    for(std::size_t i = 0; i < transf_bound[d][0]; i++)
	      {
		for(int ig = 0; ig < NGHOSTS; ig++)
		  phi[il + ig*stride_ghosts[d]] = ghosts_left [i + (ig + NGHOSTS*io) * transf_bound[d][0]];
		il += stride[d];
	      }
	    
	    for(std::size_t i = 0; i < transf_bound[d][1]; i++)
	      {
		for(int ig = 0; ig < NGHOSTS; ig++)
		  phi[ir + ig*stride_ghosts[d]] = ghosts_right[i + (ig + NGHOSTS*io) * transf_bound[d][1]];
		ir += stride[d];
	      }
	  }
      }
  }
  
 
  void test_boundaries_system() {

    /*
      This  function tests if the boudaries exchange are well implemented 
    */

    Coordinates<std::size_t, 3> z(r.Lt);
    Coordinates<std::size_t, 3> x(r.Ld);
    
    for(std::size_t  io = 0; io < (std::size_t) r.Ld[2]; io++)
      for(std::size_t i1 = NGHOSTS; i1 < (std::size_t) r.Ld[1] - NGHOSTS ; i1++)
	for(std::size_t i0 = NGHOSTS; i0 < (std::size_t) r.Ld[0] - NGHOSTS ; i0++)
	  {
	    r.convertCoordinates(z, x.set({i0,i1,io}) );
	    v(x.set({i0,i1,io}).index, 0) = aux_wr(z.index);
	  }
    
    Exchange_Boundaries();
#pragma omp barrier
    
#pragma omp critical
    {
      for(std::size_t  io = 0; io < (std::size_t) r.Ld[2]*0 + 1; io++)
	for(std::size_t i1 = 0; i1 < (std::size_t) r.Ld[1] ; i1++)
	  {
	    for(std::size_t i0 = 0; i0 < (std::size_t) r.Ld[0]; i0++)
	      {
		r.convertCoordinates(z, x.set({i0,i1,io}) );
		T val = aux_wr(z.index);
		
		if( aux_test(v(x.index , 0), val ) )
		  {
		    std::cout << "Problems---->" << v(x.index , 0) << " " << val << " ";
		    std::cout << "\t wrong " << std::real(v(x.index , 0)) << " " << z.index << " " << x.index << "\t\t";
		    x.print();
		    
		  }
	      };
	  }
    }
    std::cout << "Thread : " << r.thread_id << std::endl;
#pragma omp barrier
    exit(1);
  };

  void empty_ghosts(int mem_index) {
    /* This function takes the kpm vector that's being used, 'v' and sets to zero the part corresponding
     * to the ghosts, that is, the part of the vector that actually belongs to a different thread.
     * This is done so that when we take the dot product 'v' with another vector only terms pertraining 
     * to the current thread are considered.
     * */
  
    Coordinates<long, 3> x(r.Ld);
    
    
    // There are four sides, so set the ghosts in each side to zero individually.
    // Remember that the size of the ghost boundaries depends on NGHOSTS.
    
    for(long  io = 0; io < (long) r.Ld[2]; io++)
      for(long i0 = 0; i0 < (long) r.Ld[0]; i0++)
	for(int d = 0; d < NGHOSTS; d++)
	  v(x.set({i0,(long) d,io}).index, mem_index) *= 0;

    for(long  io = 0; io < (long) r.Ld[2]; io++)
      for(long i0 = 0; i0 < (long) r.Ld[0]; i0++)
	for(int d = 0; d < NGHOSTS; d++)
	  v(x.set({i0, (long) (r.Ld[1] - 1 - d),io}).index, mem_index) *= 0;
  
    for(long  io = 0; io < (long) r.Ld[2]; io++)
      for(long i1 = 0; i1 < (long) r.Ld[1]; i1++)
	for(int d = 0; d < NGHOSTS; d++)
	  v(x.set({(long) d,i1,io}).index, mem_index) *= 0;

    for(long  io = 0; io < (long) r.Ld[2]; io++)
      for(long i1 = 0; i1 < (long) r.Ld[1]; i1++)
	for(int d = 0; d < NGHOSTS; d++)
	  v(x.set({(long) (r.Ld[0] - 1 - d),i1,io}).index, mem_index) *= 0;

  };
  
};
      
