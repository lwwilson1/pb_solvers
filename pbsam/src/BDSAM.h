//
//  BD.h
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

#ifndef BD_h
#define BD_h

#include <stdio.h>
#include <random>
#include <memory>
#include "ElectrostaticsSAM.h"

/*
 Base class for implementing termination conditions in BD
 */
class BaseTerminateSAM
{
public:
  BaseTerminateSAM() { }
  
  virtual const bool is_terminated(shared_ptr<SystemSAM> _sys) const
  {
    return false;
  }
  
  virtual string get_how_term(shared_ptr<SystemSAM> _sys)
  {
    return "";
  }
};

class ContactTerminateSAM2 : public BaseTerminateSAM
{
protected:
  int mol1_;
  int mol2_;
  
  double pad_; // distance between spheres if contact distance cannot be met
  
  vector<vector<int> > atPairs_;  // vector of size two vectors (atom index from each MoleculeAM type)
  vector<double> dists_;  // min distance between the above pairs
  
  string how_term_;
  
public:
  ContactTerminateSAM2(vector<int> mol, vector<vector<int> > atpairs,
                    vector<double> dists, double pad)
  :BaseTerminateSAM(), mol1_(mol[0]), mol2_(mol[1]), atPairs_(atpairs),
  dists_(dists), pad_(pad)
  {
    string_create();
  }
  
  ContactTerminateSAM2(ContactFile confile, double pad)
  :pad_(pad), mol1_(confile.get_moltype1()), mol2_(confile.get_moltype2()),
  atPairs_(confile.get_at_pairs()), dists_(confile.get_dists())
  {
    string_create();
  }
  
  void string_create()
  {
    char buff[400];
    sprintf(buff, "Type %d and Type %d are in contact;\t",
            mol1_, mol2_);
    how_term_ = "System has fulfilled condition: " + string(buff);
  }
  
  string get_how_term(shared_ptr<SystemSAM> _sys)   { return how_term_; }
  
  const bool is_terminated(shared_ptr<SystemSAM> _sys) const
  {
    bool contacted = false;
    int i, j, ctct, k, idx1, idx2, sph1, sph2;
    Pt cen1, cen2, vc1, vc2;
    double a1, a2, dcon;

    for ( i = 0; i < _sys->get_typect(mol1_); i++)
    {
      for ( j = 0; j < _sys->get_typect(mol2_); j++)
      {
        ctct = 0;
        idx1 = _sys->get_mol_global_idx( mol1_, i);
        idx2 = _sys->get_mol_global_idx( mol2_, j);
        
        for (k = 0; k < atPairs_.size(); k++)
        {
          dcon = dists_[k];
          
          sph1 = _sys->get_moli(idx1)->get_cg_of_ch(atPairs_[k][0]);
          sph2 = _sys->get_moli(idx1)->get_cg_of_ch(atPairs_[k][0]);
          
          cen1 = _sys->get_centerik(idx1, sph1);
          cen2 = _sys->get_centerik(idx2, sph2);
          
          a1 = _sys->get_aik(idx1, sph1);
          a2 = _sys->get_aik(idx2, sph2);

          // if the distance between the 2 CG spheres is less than the cutoff
          // The points are in contact
          if ( _sys->get_pbc_dist_vec_base( cen1, cen2).norm() < a1+a2+dcon)
          {
            cout << "This is dcon " << dcon << " sph1 " << cen1.x() << " " <<
            cen1.y() << " " << cen1.z() << " " << " & sph2 " << cen2.x()
            << " " << cen2.y() << " " << cen2.z() << " " << endl;
            ctct++;
            if (ctct == 2){contacted = true; break;}
          }
        }
      }
    }
    return contacted;
  }
};


/*
 Class for time based termination
 */
class TimeTerminateSAM : public BaseTerminateSAM
{
protected:
  double endTime_; //termination time
  string how_term_;
  
public:
  TimeTerminateSAM(double end_time)
  :BaseTerminateSAM(), endTime_(end_time)
  {
    char buff[400];
    sprintf(buff, "System has fulfilled condition: time >= %7.1f;\t", endTime_);
    how_term_ = buff;
  }
  
  const bool is_terminated(shared_ptr<SystemSAM> _sys) const
  {
    bool done = false;
    if (_sys->get_time() >= endTime_) done = true;
    return done;
  }
  
  string get_how_term(shared_ptr<SystemSAM> _sys)   { return how_term_; }
};

enum CoordType { X, Y, Z, R };
enum BoundaryType { LEQ, GEQ };

/*
 Class for coordinate based termination. This terminates based on whether
 the specified MoleculeAM satisfies the BoundaryType condition on the CoordType
 with the given boundary value.
 */
class CoordTerminateSAM : public BaseTerminateSAM
{
protected:
  double boundaryVal_;
  int molIdx_;
  CoordType coordType_;
  BoundaryType boundType_;
  string how_term_;
  
public:
  CoordTerminateSAM(int mol_idx, CoordType coord_type,
                 BoundaryType bound_type, double bound_val)
  :BaseTerminateSAM(), molIdx_(mol_idx), coordType_(coord_type),
  boundType_(bound_type), boundaryVal_(bound_val)
  {
    char buff[400];
    string cord = "r", eq = ">=";
    if (coordType_ == X)      cord = "x";
    else if (coordType_ == Y) cord = "y";
    else if (coordType_ == Z) cord = "z";
    
    if (boundType_ == LEQ)    eq   = "<=";
    
    sprintf(buff, "MoleculeAM type %d has fulfilled condition: %s %s %5.2f;\t",
            molIdx_, cord.c_str(), eq.c_str(), boundaryVal_);
    how_term_ = buff;
  }
  
  const bool is_terminated(shared_ptr<SystemSAM> _sys) const
  {
    bool done = false;
    int i, idx;
    for ( i = 0; i < _sys->get_typect(molIdx_); i++)
    {
      idx = _sys->get_mol_global_idx( molIdx_, i);
      Pt mol_coord = _sys->get_unwrapped_center(idx);
      double test_val;
      if (coordType_ == X)      test_val = mol_coord.x();
      else if (coordType_ == Y) test_val = mol_coord.y();
      else if (coordType_ == Z) test_val = mol_coord.z();
      else                      test_val = mol_coord.norm();
      
      if ((boundType_ == LEQ) && (test_val <= boundaryVal_))      return true;
      else if ((boundType_ == GEQ) && (test_val >= boundaryVal_)) return true;
    }
    return done;
  }
  
  string get_how_term(shared_ptr<SystemSAM> _sys)   { return how_term_; }
};


/*
 Combine termination conditions
 */
enum HowTermCombineSAM { ALL, ONE };

class CombineTerminateSAM: public BaseTerminateSAM
{
protected:
  vector<shared_ptr<BaseTerminateSAM> > terms_;
  HowTermCombineSAM howCombine_;
  
public:
  CombineTerminateSAM(vector<shared_ptr<BaseTerminateSAM> > terms,
                      HowTermCombineSAM how_combine)
  :BaseTerminateSAM(), terms_(terms), howCombine_(how_combine)
  {
  }
  
  const bool is_terminated(shared_ptr<SystemSAM> _sys) const
  {
    bool done;
    howCombine_== ALL ? done=true : done=false;
    for (int i = 0; i < terms_.size(); i++)
    {
      if (terms_[i]->is_terminated(_sys) == ! done)
      {
        done=!done;
        break;
      }
    }
    return done;
  }
  
  string get_how_term(shared_ptr<SystemSAM> _sys)
  {
    string how_term = "";
    bool done;
    howCombine_== ALL ? done=true : done=false;
    for (int i = 0; i < terms_.size(); i++)
    {
      if (terms_[i]->is_terminated(_sys) == true)
      {
        how_term += terms_[i]->get_how_term(_sys);
      }
    }
    return how_term;
  }
};

/*
 Class for performing a brownian dynamics step
 */
class BDStepSAM
{
protected:
  vector<double> transDiffConsts_;  // translational diffusion constants
  vector<double> rotDiffConsts_;  // rotational diffusion constants
  
  bool diff_; // include random kicks in dynamics
  bool force_; // include force calcs in dynamics
  double dt_;
  double min_dist_;
  
  // random number generator object:
  mt19937 randGen_;
  shared_ptr<SystemSAM> _sys_;
  shared_ptr<Constants> _consts_;
  
  // check if a MoleculeAM's new point causes it to collide with any other
//  bool check_for_collision(int mol, Pt new_pt);
  
  // updates on individual MoleculeAMs:
  void indi_trans_update(int i, Pt fi);
  void indi_rot_update(int i, Pt tau_i);
  
  // compute timestep for BD
  double compute_dt( );
  
  // compute the smallest distance between two MoleculeAM centers
  void compute_min_dist( );
  
  // return a random vector with each element drawn from a Gaussian
  Pt rand_vec(double mean, double var);
  
  // update System time
  void update_sys_time(double dt) { _sys_->set_time(_sys_->get_time() + dt); }
  
public:
  BDStepSAM(shared_ptr<SystemSAM> _sys, shared_ptr<Constants> _consts,
     vector<double> trans_diff_consts,
     vector<double> rot_diff_consts,
     bool diff = true, bool force = true);
  
  // Constructor where diffusion constants are read from system:
  BDStepSAM(shared_ptr<SystemSAM> _sys, shared_ptr<Constants> _consts,
         bool diff = true, bool force = true);
  
  // update the system with Brownian dynamics given forces and torques on every
  // MoleculeAM
  void bd_update(shared_ptr<vector<Pt> > _F,
                 shared_ptr<vector<Pt> > _tau);
  
  shared_ptr<SystemSAM> get_system() { return _sys_; }
  double get_dt()                 { return dt_; }
  double get_min_dist()           { return min_dist_; }
  
};


/*
 Class for running a full BD simulation
 */
class BDRunSAM
{
protected:
  shared_ptr<BDStepSAM> _stepper_;
  shared_ptr<Solver> _solver_;
  shared_ptr<GradSolver> _gradSolv_;
  shared_ptr<BasePhysCalcSAM> _physCalc_;
  shared_ptr<BaseTerminateSAM> _terminator_;
  
  string outfname_; //outputfile
  
  int maxIter_;
  double prec_;
  
public:
  // num is the number of bodies to perform calculations on (2, 3 or all).
  // If num=0, then the equations will be solved exactly
  BDRunSAM(shared_ptr<Solver> _solv, shared_ptr<GradSolver> _gradSolv,
        shared_ptr<BaseTerminateSAM> _terminator, string outfname, int num = 0,
        bool diff = true, bool force = true,
        int maxiter = 2, double prec = 1e-4);
  
  void run(string xyzfile = "test.xyz", string statfile = "stats.dat", 
           int nSCF = 0);
};




#endif /* BD_h */
