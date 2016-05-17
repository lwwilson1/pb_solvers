//
//  System_hpp
//  pb_solvers_code
//
/*
 Copyright (c) 2015, Teresa Head-Gordon, Lisa Felberg, Enghui Yap, David Brookes
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of UC Berkeley nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef System_hpp
#define System_hpp

#include <map>
#include "Constants.h"

using namespace std;


class CGSphere
{
protected:
  Pt cen_;
  double a_;
//  int n_; 
  vector<int> ch_;  // encompassed charges
  
public:
  CGSphere() { }
  CGSphere(Pt cen, double a, int n, vector<int> ch)
  : cen_(cen), a_(a), ch_(ch) { }
  
  Pt get_center() const { return cen_; }
  double get_a() const  { return a_; }
  int get_n() const     { return (int) ch_.size(); }
  vector<int> get_ch() const { return ch_; }
  
//  void set_center(Pt cen) { cen_ = cen; }
//  void set_a(double a) { a_ = a; }
  
};


class Molecule
{
protected:
  string              moveType_;
  int                 type_; // int index of type of molecule, 0 based
  int                 typeIdx_; // int index of mol within given type_, 0 based
  double              drot_;  // rotational diffusion coefficient
  double              dtrans_; // translational diffusion coefficients
  
  int                 Nc_;  // number of charges in this molecule
  vector<double>      qs_;  // magnitude of each charge in the molecule
  vector<Pt>          pos_;  // position of each charge in the molecule
  vector<double>      vdwr_; // van der waal radius of each chargeGoogle
  
  int                     Ns_;  // number of coarse grained spheres
  vector<Pt>              centers_; //coarse-grained sphere centers
  vector<double>          as_; // coarse-grained sphere radii
  vector<vector<int> >    cgCharges_; // list of indices of charges within each
                                      // coarse grained sphere
  map<int, int>           chToCG_; // maps index of charge to
                                    //index of its coarse-grained sphere
  
  int find_closest_center(Pt pos);
  
  /*
   Function that chooses centers for charges in the molecule
   */
  void find_centers(vector<Pt> sp, vector<Pt> np,
                    double tol_sp, int n_trials,
                    int max_trials);
  
  /*
   Perform a monte carlo search for the best center to encompass the 
   remaining charges
   */
  CGSphere find_best_center(vector<Pt> sp,vector<Pt> np,
                            vector<int> unbounded,
                            double tol_sp, int iter=1200, double beta=2.0);
  
public:
  
  Molecule() { }
  
  Molecule(const Molecule& mol);
  
  // user specified centers
  Molecule(int type, int type_idx, string movetype, vector<double> qs,
             vector<Pt> pos, vector<double> vdwr, vector<Pt> cens,
             vector<double> as, double drot=0, double dtrans=0);
  
  Molecule(int type, int type_idx, string movetype, vector<double> qs,
             vector<Pt> pos, vector<double> vdwr, vector<Pt> msms_sp,
             vector<Pt> msms_np, double tol_sp, int n_trials=1200,
             int max_trials=40, double beta=2.0, double drot=0,
             double dtrans=0);
  
  void set_type_idx(int typeidx) { typeIdx_ = typeidx; }
  
  
  void translate(Pt dr, double boxlen);
  void rotate(Quat qrot);
  
  string get_move_type() const          { return moveType_; }
  int get_type() const                  { return type_; }
  int get_type_idx() const              { return typeIdx_; }
  int get_nc() const                    { return Nc_; }
  int get_ns() const                    { return Ns_; }
  double get_drot() const               { return drot_; }
  double get_dtrans() const             { return dtrans_; }
  Pt get_posj(int j) const              { return pos_[j]; }
  Pt get_posj_realspace(int j)          { return pos_[j] + centers_[chToCG_[j]];}
  const double get_qj(int j) const      { return qs_[j]; }
  const double get_radj(int j) const    { return vdwr_[j]; }
  Pt get_centerk(int k) const           { return centers_[k]; }
  const double get_ak(int k) const      { return as_[k]; }
  
  /*
   Choose a random orientation for a Pt vector
   */
  Pt random_pt();
  
  // random number from normal distribution
  double random_norm();
  
};

/*
 Class containing all of the relevant information for a particular system
 */
class System
{
protected:
    
  int                          N_; // number of molecules
  double                       lambda_; // average molecular radius
  vector<Molecule>             molecules_;
  
  double                       boxLength_;
  double                       cutoff_;
  
  double t_;  // time in a BD simulation
  
  int                          ntype_;  //count of unique molecule types
  vector<int>                  typect_; //count of molecule of each unique type
  map<vector<int>, int>        typeIdxToIdx_;
  
  const double calc_average_radius() const;
  
//  Molecule build_type_mol(int type, Setup setup);
  
public:
  System() { }
  
  System(const vector<Molecule>& mols,
         double cutoff=Constants::FORCE_CUTOFF,
         double boxlength=Constants::MAX_DIST);
  
  System(Setup setup, double cutoff=Constants::FORCE_CUTOFF);
  
  const int get_n() const                  {return N_;}
  const int get_ntype()                    {return ntype_;}
  const int get_typect(int i)              {return typect_[i];}
  const double get_aik(int i, int k) const {return molecules_[i].get_ak(k);}
  const double get_Nc_i(int i) const       {return molecules_[i].get_nc();}
  const double get_Ns_i(int i) const       {return molecules_[i].get_ns();}
  const double get_qij(int i, int j) const {return molecules_[i].get_qj(j);}
  const double get_droti(int i) const      {return molecules_[i].get_drot();}
  const double get_dtransi(int i) const    {return molecules_[i].get_dtrans();}
  const double get_boxlength() const       {return boxLength_;}
  const double get_cutoff() const          {return cutoff_;}
  const double get_time() const            {return t_;}
  const double get_lambda() const          {return lambda_;}
  Molecule get_molecule(int i) const       {return molecules_[i];}
  Pt get_posij(int i, int j)               {return molecules_[i].get_posj(j);}
  Pt get_centerik(int i, int k) const    {return molecules_[i].get_centerk(k);}
  const string get_typei(int i) const    {return molecules_[i].get_move_type();}
  
  const double get_radij(int i, int j) const {return molecules_[i].get_radj(j);}
  Pt get_posijreal(int i, int j) {return molecules_[i].get_posj_realspace(j);}
  
  const int get_mol_global_idx(int type, int ty_idx)
  {
    vector<int> keys = {type, ty_idx};
    return typeIdxToIdx_[keys];
  }
  
  // Compute cutoff for force calcs
  void compute_cutoff();
  
  // Set time of simulation as what is input
  void set_time(double val) { t_ = val; }
  
  // translate every charge in molecule i by the vector dr
  void translate_mol(int i, Pt dr) { molecules_[i].translate(dr, boxLength_); }
  
  // rotate every charge in molecule i
  void rotate_mol(int i, Quat qrot) { molecules_[i].rotate(qrot); }
  
  // Check to determine if any molecules are overlapping
  void check_for_overlap();
  
  // get distance vector between any two points taking into account periodic
  // boundary conditions
  Pt get_pbc_dist_vec_base(Pt p1, Pt p2);
  
  // given a distance vector, determine whether it is in the cutoff
  bool less_than_cutoff(Pt v);
  
  // reset positions with input xyz file
//  void reset_positions( vector<string> xyzfiles );
  
  // write current system to PQR file
  void write_to_pqr( string outfile );
  
  // write current system configuration to XYZ file
  void write_to_xyz(ofstream &xyz_out);
  
};

/*
 Exception thrown when two molecules in the system are overlapping
 */
class OverlappingMoleculeException: public exception
{
protected:
  int idx1_;
  int idx2_;
  
public:
  OverlappingMoleculeException(int idx1, int idx2)
  :idx1_(idx1), idx2_(idx2)
  {
  }
  
  virtual const char* what() const throw()
  {
    string ss;
    ss = "Molecule " + to_string(idx1_)+" & " + to_string(idx2_) + " overlap";
    return ss.c_str();
  }
};

#endif /* Setup_hpp */
