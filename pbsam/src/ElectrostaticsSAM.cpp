//
//  Electrostatics.cpp
//  pb_solvers_code
//
//  Created by Lisa Felberg on 3/3/16.
//  Copyright © 2016 Lisa Felberg. All rights reserved.
//

#include "ElectrostaticsSAM.h"

ElectrostaticSAM::ElectrostaticSAM(vector<shared_ptr<HMatrix> > H,
                             shared_ptr<SystemSAM> _sys,
                             shared_ptr<SHCalc> _shCalc,
                             shared_ptr<BesselCalc> _bCalc,
                             shared_ptr<Constants> _consts,
                             int p, int npts)
: p_(p), pot_min_(0), pot_max_(0), _H_(H), _sys_(_sys), _shCalc_(_shCalc),
_bCalc_(_bCalc), _consts_(_consts), lam_(_sys->get_lambda())
{
  range_min_.resize(3);
  range_max_.resize(3);
  npts_.resize(3);
  step_.resize(3);
  
  for (int i = 0; i < 3; i++)
    npts_[i] = npts;
  
  grid_.resize(npts);
  for (int i = 0; i < grid_.size(); i++)
    grid_[i].resize(npts);
  
  find_range();
  find_bins();
  
  compute_units();
  compute_pot();

}

ElectrostaticSAM::ElectrostaticSAM(shared_ptr<Solver> solve, int npts)
:p_(solve->get_p()), pot_min_(0), pot_max_(0),
lam_(solve->get_sys()->get_lambda()), _H_(solve->get_all_H()),
_sys_(solve->get_sys()), _shCalc_(solve->get_sh()),
_bCalc_(solve->get_bessel()), _consts_(solve->get_consts())
{
  range_min_.resize(3);
  range_max_.resize(3);
  npts_.resize(3);
  step_.resize(3);
  
  for (int i = 0; i < 3; i++)
    npts_[i] = npts;
  
  grid_.resize(npts);
  for (int i = 0; i < grid_.size(); i++)
    grid_[i].resize(npts);
  
  find_range();
  find_bins();
  
  compute_units();
  
  cout << setprecision(9) <<" This is units " << units_ << " my range " << range_min_[0]
  <<  ", " <<range_min_[1]
  <<  ", "<<range_min_[2] <<  " and max " << range_max_[0] <<  ", "
  <<range_max_[1] <<  ", "<<range_max_[2] << "  bins "  << step_[0] <<  ", "
  <<step_[1] <<  ", "<<step_[2] << "  bins "  << npts_[0] <<  ", " <<npts_[1]
  <<  ", "<<npts_[2] << endl;
  
  compute_pot();
}


void ElectrostaticSAM::find_range()
{
  int mol, sph, dim;
  Pt center, curAt;
  double rad;
  double point[3] = {0,0,0}; double max[3] = {0,0,0}; double min[3] = {0,0,0};
  
  int Nmol = _sys_->get_n();
  for ( mol = 0; mol < Nmol; mol++)
  {
    for ( sph = 0; sph < _sys_->get_Ns_i(mol); sph++)
    {
      center = _sys_->get_centerik(mol, sph);
      rad    = _sys_->get_aik(mol, sph);
      point[0] = center.x(); point[1] = center.y(); point[2] = center.z();
      for (dim = 0; dim < 3; dim++)
      {
        if(point[dim]-rad < min[dim]) min[dim] = point[dim]-rad;
        if(point[dim]+rad > max[dim]) max[dim] = point[dim]+rad;
      }
    }
  }
  
  for (dim = 0; dim<3; dim++)
  {
    range_min_[dim] = min[dim] - lam_;
    range_max_[dim] = max[dim] + lam_;
  }
}


void ElectrostaticSAM::find_bins()
{
  int dim, x, y;
  
  for (dim = 0; dim<3; dim++)
    step_[dim] = (range_max_[dim] - range_min_[dim]) / (double) npts_[dim];
  
  esp_.resize(npts_[0]);
  for ( x = 0; x < npts_[1]; x++)
  {
    esp_[x].resize(npts_[1]);
    for ( y = 0; y < npts_[2]; y++)
      esp_[x][y].resize(npts_[2]);
  }
}

void ElectrostaticSAM::compute_units()
{
  units_ = _consts_->get_conv_factor();
}

void ElectrostaticSAM::print_dx( string dxname )
{
  ofstream dx;
  char pot[20];
  int xct, yct, zct;
  int ct = 0;
  double out, pot_max_ = 0;
  double pot_min_ = 0;
  
  dx.open(dxname);
  dx << "# Data from PBSAM Electrostat run" << endl;
  dx << "# My runname is " << dxname << " and units " ;
  dx << _consts_->get_units() <<  endl;
  dx << "object 1 class gridpositions counts " << npts_[0]
     << " " << npts_[1] << " " << npts_[2] << endl;
  dx << "origin " << range_min_[0] << " " << range_min_[1]
     << " " << range_min_[2] << endl;
  dx << "delta " << step_[0] << " 0.0e+00 0.0e+00" << endl;
  dx << "delta 0.0e00 " << step_[1] << " 0.0e+00" << endl;
  dx << "delta 0.0e00 0.0e+00 " << step_[2] << endl;
  dx << "object 2 class gridconnections counts " << npts_[0]
    << " " << npts_[1] << " " << npts_[2] << endl;
  dx << "object 3 class array type double rank 0 items "
    << npts_[0]*npts_[1]*npts_[2] << " data follows" << endl;
  
  for ( xct=0; xct<npts_[0]; xct++)
  {
    for ( yct=0; yct<npts_[1]; yct++)
    {
      for ( zct=0; zct<npts_[2]; zct++)
      {
        out = ((esp_[xct][yct][zct] != esp_[xct][yct][zct])
                ? 0.0 : esp_[xct][yct][zct]);
        sprintf( pot, "%12.9f ", out);
        dx << pot;
        
        ct++;
        if ((ct % 5) == 0) dx << "\n";
        
        if (esp_[xct][yct][zct] < pot_min_)      pot_min_ = esp_[xct][yct][zct];
        else if (esp_[xct][yct][zct] > pot_max_) pot_max_ = esp_[xct][yct][zct];
        
      }
    }
  }
  
  dx << endl; dx << "attribute \"dep\" string \"positions\"" << endl;
  dx << "object \"regular positions regular connections\" class field" << endl;
  dx << "component \"positions\" value 1 " << endl;
  dx << "component \"connections\" value 2" << endl;
  dx << "component \"data\" value 3" << endl;
  dx.close();
  
  cout << "This is min " << pot_min_ << endl;
  cout << "This is max " << pot_max_ << endl;
}

void ElectrostaticSAM::print_3d_heat( string td_name )
{
  ofstream ht;
  char pot[500];
  Pt center, pos;
  int xct, mol, sph, Nmol = _sys_->get_n();
  double ptl, e_s(_consts_->get_dielectric_water());
  
  ht.open(td_name);
  ht << "# Data from PBSAM Electrostat run" << endl;
  ht << "# My runname is " << td_name << " and units " ;
  ht << _consts_->get_units() <<  endl;
  ht << "grid " << npts_[0] << " " << npts_[1] << " " << npts_[2] << endl;
  ht << "origin " << range_min_[0] << " " << range_min_[1]
     << " " << range_min_[2] << endl;
  ht << "delta " << step_[0] << " " << step_[1] << " " << step_[2] << endl;

  for ( mol = 0; mol < Nmol; mol++)
  {
    for ( sph = 0; sph < _sys_->get_Ns_i(mol); sph++)
    {
      vector<int> exp_pts = _sys_->get_moli(mol)->get_gdpt_expj(sph);
      center = _sys_->get_centerik(mol, sph);
      for (xct = 0; xct < exp_pts.size(); xct++)
      {
        pos = _sys_->get_moli(mol)->get_gridjh(sph,exp_pts[xct]) + center;
        pos = Pt( pos.r()*1.05, pos.theta(), pos.phi(), true);
        ptl = (units_*compute_pot_at(pos))/e_s;
        
        sprintf(pot, "%12.7f %12.7f %12.7f %12.7f ",
                pos.x(), pos.y(), pos.z(), ptl);
        ht << pot << endl;
      }
    }
  }
  
  ht.close();
}

void ElectrostaticSAM::print_grid(string axis, double value, string fname)
{
  int i, j, idx = 0;
  double v_act;
  ofstream f;
  char pot[20];
  vector<double> org(2), delta(2);
  
  if (( axis == "x" ) and (value < range_min_[0]))
    throw ValueOutOfRange( axis, value, range_min_[0] );
  if (( axis == "x" ) and (value > range_max_[0]))
    throw ValueOutOfRange( axis, value, range_max_[0] );
  if (( axis == "y" ) and (value < range_min_[1]))
    throw ValueOutOfRange( axis, value, range_min_[1] );
  if (( axis == "y" ) and (value > range_max_[1]))
    throw ValueOutOfRange( axis, value, range_max_[1] );
  if (( axis == "z" ) and (value < range_min_[2]))
    throw ValueOutOfRange( axis, value, range_min_[2] );
  if (( axis == "z" ) and (value > range_max_[2]))
    throw ValueOutOfRange( axis, value, range_max_[2] );

  for (i = 0; i < grid_.size(); i++)
    for (j = 0; j < grid_[0].size(); j++)
    {
      if (axis ==  "x")
      {
        idx = round((value-range_min_[0]) / step_[0]);
        grid_[i][j] = esp_[idx][i][j];
        
      } else if (axis ==  "y")
      {
        idx = round((value-range_min_[1]) / step_[1]);
        grid_[i][j] = esp_[i][idx][j];
      } else
      {
        idx = round((value-range_min_[2]) / step_[2]);
        grid_[i][j] = esp_[i][j][idx];
      }
      
      if (grid_[i][j] < pot_min_)      pot_min_ = grid_[i][j];
      else if (grid_[i][j] > pot_max_) pot_max_ = grid_[i][j];
    }
  
  if ( axis == "x" )
  {
    org[0] = range_min_[1]; org[1] = range_min_[2];
    delta[0] = step_[1]; delta[1] = step_[2];
    v_act = idx * step_[0] + range_min_[0];
  }
  else if ( axis == "y" )
  {
    org[0] = range_min_[0]; org[1] = range_min_[2];
    delta[0] = step_[0]; delta[1] = step_[2];
    v_act = idx * step_[1] + range_min_[1];
  } else
  {
    org[0] = range_min_[0]; org[1] = range_min_[1];
    delta[0] = step_[0]; delta[1] = step_[1];
    v_act = idx * step_[2] + range_min_[2];
  }

  f.open(fname);
  v_act = (abs(v_act) < 1e-12) ? 0 : v_act;
  f << "# Data from PBSAM Electrostat run\n# My runname is " << fname << endl;
  f << "units " << _consts_->get_units() <<  endl;
  f << "grid " << grid_.size() << " " << grid_[0].size() << endl;
  f << "axis " << axis << " " << v_act << endl;
  f << "origin " << org[0] << " " << org[1] << endl;
  f << "delta " << delta[0] << " " << delta[1] << endl;
  f << "maxmin " << pot_max_ << " " << pot_min_ << endl;
  
  for (i = 0; i < grid_.size(); i++)
  {
    for (j = 0; j < grid_[0].size(); j++)
    {
      sprintf( pot, "%12.7f  ", grid_[i][j]);
      f << pot;
    }
    f << "\n";
  }
  f.close();
}

void ElectrostaticSAM::compute_pot()
{
  int Nmol = _sys_->get_n();
  double e_s = _consts_->get_dielectric_water();
  clock_t t;
  t = clock();
  int total = 0;
  #pragma omp parallel for
  for ( int xct=0; xct<npts_[0]; xct++)
  {
    cout  << range_min_[0]+xct*step_[0] << " ..  " << endl ;
    for (int yct=0; yct<npts_[1]; yct++)
    {
      for ( int zct=0; zct<npts_[2]; zct++)
      {
        #pragma omp atomic
        total += 1;

//        if (total % 100000 == 0) cout << total << "\t" << ((float) (clock() - t ))/CLOCKS_PER_SEC <<  endl;
        
        Pt center, pos;
        bool cont(false);
        double rad;
        int mol, sph;
        for ( mol = 0; mol < Nmol; mol++)
        {
          for ( sph = 0; sph < _sys_->get_Ns_i(mol); sph++)
          {
            center = _sys_->get_centerik(mol, sph);
            rad    = _sys_->get_aik(mol, sph);
            pos = Pt(range_min_[0]+xct*step_[0], range_min_[1]+yct*step_[1],
                     range_min_[2]+zct*step_[2]);
            
            if ((pos - center).norm2() < rad*rad)
            {
              cont = true;
              break;
            }
          }
        }
        #pragma omp critical
        {
        if (cont)
          esp_[xct][yct][zct] = NAN;
        else
          esp_[xct][yct][zct] = (units_*compute_pot_at(pos))/e_s;
        }
      }
    }
  }
  t = clock() - t;
  printf ("compute_pot() took me %lu clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
  
}

double ElectrostaticSAM::compute_pot_at( Pt point )
{

  int mol, sph, Nmol = _sys_->get_n();
  double rad, pot    = 0.0;
  Pt center, dist;
  MyMatrix<cmplx> localK;
  
  for ( mol = 0; mol < Nmol; mol++)
  {
    for ( sph = 0; sph < _sys_->get_Ns_i(mol); sph++)
    {
      center = _sys_->get_centerik(mol, sph);
      rad    = _sys_->get_aik(mol, sph);
      dist   = point - center;
      localK = get_local_exp(dist, rad);
      pot += lotan_inner_prod( _H_[mol]->get_mat_k(sph), localK, p_);
    }
  }
  return pot;
}

MyMatrix<cmplx> ElectrostaticSAM::get_local_exp( Pt dist, double lambda )
{
  int n, m;
  double expKR, kap    = _consts_->get_kappa();
  vector<double> bessK;
  MyMatrix<cmplx> localK(p_, 2*p_);
  
  bessK = _bCalc_->calc_mbfK(p_, kap*dist.r());
  expKR = exp( - kap * dist.r()) / dist.r();
  _shCalc_->calc_sh(dist.theta(),dist.phi());
  
  for ( n = 0; n < p_; n++)
  {
    for ( m = -n; m <= n; m++)
    {
      localK.set_val( n, m+p_, (pow( lambda/dist.r(), n) * expKR *
                                _shCalc_->get_result(n, m) * bessK[n]));
    }
  }
  return localK;
}

double ElectrostaticSAM::lotan_inner_prod(MyMatrix<cmplx> U, MyMatrix<cmplx> V,
                                       int p)
{
  double ip = 0;
  int n, m, mT;
  for (n = 0; n < p; n++)
  {
    for (m = -n; m <= n; m++)
    {
      mT = (m < 0) ? -1*m : m;
      ip += (U(n, mT+p_).real()*V(n, mT+p_).real()
             + U(n, mT+p_).imag()*V(n, mT+p_).imag());
    }
  }
  return ip;
}

