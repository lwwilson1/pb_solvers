//
//  ASolverUnitTest.h
//  pbsolvers
//
//  Created by Marielle Soniat on 10/6/15.
//  Copyright (c) 2015 Marielle Soniat. All rights reserved.
//

#ifndef pbsolvers_ASolverUnitTest_h
#define pbsolvers_ASolverUnitTest_h

#include "ASolver.h"

class ASolverUTest : public ::testing::Test
{
public :
  
protected :
  
  int vals_;
  Constants const_;
  vector< Molecule > mol_;
  vector< Molecule > mol_sing_;
  
  virtual void SetUp()
  {
    mol_.clear( );
    Pt pos[2]     = { Pt( 0.0, 0.0, -5.0 ), Pt( 10.0, 7.8, 25.0 ) };
    Pt cgPos[2]   = { Pt( 0.0, 0.0, -5.5 ), Pt( 11.0, 6.9, 24.3 ) };
    double cg[2] = { 5.0, -0.4};
    double rd[2] = { 5.6, 10.4};
    
    Pt cgPosSi[2] = { Pt( 0.0, 0.0, -35.0 ), Pt( 0.0, 0.0, -0.0 ) };
    
    for (int molInd = 0; molInd < 2; molInd ++ )
    {
      int M = 1;
      vector<double> charges(1);
      vector<Pt> posCharges(1);
      
      charges[0]    = cg[molInd];
      posCharges[0] = cgPos[molInd];
      
      Molecule molNew( M, rd[molInd], charges, posCharges, pos[molInd]);
      mol_.push_back( molNew );
      
      charges[0]    = 2.0;
      posCharges[0] = cgPosSi[molInd];

      Molecule molSing( M, 10.0, charges, posCharges);
      mol_sing_.push_back( molSing );
    }
  } // end SetUp
  
  virtual void TearDown() {}
  
//  double A0[15] = {5.06492124, -3.6629753, -0.000641312455, 2.01744339,
//    -0.000150832137,-5.86932378e-06,-1.05487251,-1.46904154e-05,8.44898379e-07,
//    8.34955966e-07, 0.541011058, 4.98503746e-07, 9.23014214e-07, 1.77640914e-07,
//    3.51977104e-09,};
//  
//  double A0_im[15] = { -4.51751614e-09, -0.000253764627, -0.000546361723,
//    -0.000140256989, -0.000161954107, -4.28036153e-05, -3.85518402e-05,
//    -2.87295411e-05, -8.62026893e-06, -9.90550946e-07, -7.87063036e-06,
//    -4.16328095e-06, -5.59013e-07, 8.23515067e-08, 2.35969719e-08};
//  
//   double A1[15] = { -0.402210927, 0.272084187, -0.448632042, 0.332513859,
//     0.584393585, -0.0735997277, -1.07437691, -0.058387728, 0.121844253,
//     0.542085518, 0.832263495, -0.937549973, -0.0580719897, -1.02883583,
//     1.16366737};
//  
//  double A1_im[15] = {-6.00898647e-07,1.0270678e-05,0.348521971,-9.72430906e-06,
//    -0.478199881, 0.722212407, 6.23391427e-06, 0.0283916673, -1.17520566,
//    0.745545848, -3.18354738e-06, 0.853352497, 0.562026511, -1.41661515,
//    0.248528357};

  double A0[15] = { 5.06496536, -3.6619316, -0.000503250061, 2.01779987,
    -9.82900676e-05, -1.0682227e-05, -1.05479692, -3.28880159e-06,
    -2.71180395e-06, -4.4780814e-07, 0.541023412, 2.40308854e-06,
    -3.44083312e-07, -4.12058842e-07, -8.58237216e-08};
  
  double A0_im[15] = {0, 0.000340562962, -0.000330632776, 0.000163525329,
    -2.30829341e-05, -1.86457723e-05, 3.5928091e-05, 1.53052678e-05,
    2.2200138e-06, 9.73474763e-09, 5.13843793e-06, 5.61706783e-06,
    2.48309863e-06, 4.65173384e-07, 2.04983496e-08 };
  
  double A1[15] = {-0.401833604, 0.265700049, -0.449876813, 0.338288496,
    0.586157705,-0.0734981144, -1.07786325, -0.0598377042, 0.12171614,
    0.54211679, 0.83395858, -0.93662999, -0.057964058, -1.028876, 1.1636555};
  
  double A1_im[15] = {0, 4.58511161e-06, 0.347557646, -8.25770523e-06,
    -0.47683061, 0.722606727, 7.39004703e-06, 0.0272651594, -1.17570528,
    0.74547344,-4.85359351e-06, 0.85406746, 0.56244862,-1.4165226, 0.24853490};
  
  double A0Sing[15] = {2.08493611, 0.0595090739,0, 0.026460114, 0, 0,
    0.00906667786, 0, 0, 0, 0.00287090794, 0, 0, 0, 0};
  
  double A1Sing[15] = {2.08493623, -0.059510451, 0, 0.026460955, 0, 0,
    -0.00906706693, 0, 0, 0, 0.00287106903, 0, 0, 0, 0};
  
  double dA00[15] = {1.0544637e-06,7.85368589e-05,-3.7739641e-05,1.1710611e-05,
    -4.2581267e-06,-2.54957295e-06,1.1509835e-06,-2.02804679e-07,-3.6652067e-07,
    -4.9837825e-08,8.9749664e-08,2.9501223e-09,-3.6136045e-08,-8.8566072e-09,0};
  
  double dA01im[15] = {0,0,4.49886716e-05,0,5.90910631e-06,1.96972506e-06,0,
    4.233064e-07,2.48485723e-07,5.45274128e-09,0,1.98818687e-08,1.98691187e-08,
    0,-2.28180158e-09};
  
  double dA11[15] = {-3.1633912e-06,0.00015605884,5.5533741e-05,-1.438732e-05,
    -9.82898223e-06,-8.25866317e-07,8.00828799e-07,1.0218887e-06,
    1.40543141e-07,-4.82103708e-08,-2.17563271e-08,-8.19726265e-08,
    -1.62881095e-08,8.72542217e-09,3.2623144e-09};
  
  double dA10im[15] = {0,0,-1.44387654e-05,0,3.28980681e-06,-1.10667758e-06,0,
    -4.39392408e-07,1.06332252e-07,9.822109e-08,0,4.54976027e-08,-3.4394996e-09,
    -1.4438813e-08,-3.54879966e-09};

} ; // end ASolverUTest


TEST_F(ASolverUTest, checkGamma)
{
  const int vals           = nvals;
  int nmol                 = 2;
  BesselConstants bConsta  = BesselConstants( 2*vals );
  BesselCalc bCalcu        = BesselCalc( 2*vals, bConsta );
  SHCalcConstants SHConsta = SHCalcConstants( 2*vals );
  SHCalc SHCalcu           = SHCalc( 2*vals, SHConsta );
  System sys               = System( const_, mol_ );
  ReExpCoeffsConstants re_exp_consts (sys.get_consts().get_kappa(),
                                      sys.get_lambda(), nvals);
  
  ASolver ASolvTest        = ASolver( nmol, vals, bCalcu, SHCalcu, sys);

  EXPECT_NEAR( ASolvTest.get_gamma_ni( 0, 1).real(),  1.463995711, preclim);
  EXPECT_NEAR( ASolvTest.get_gamma_ni( 0, 5).real(),  1.760111936, preclim);
  
  EXPECT_NEAR( ASolvTest.get_gamma_ni( 1, 2).real(),  1.621243794, preclim);
  EXPECT_NEAR( ASolvTest.get_gamma_ni( 1, 7).real(),  1.799701878, preclim);
}

TEST_F(ASolverUTest, checkDelta)
{
  const int vals           = nvals;
  int nmol                 = 2;
  BesselConstants bConsta  = BesselConstants( 2*vals );
  BesselCalc bCalcu        = BesselCalc( 2*vals, bConsta );
  SHCalcConstants SHConsta = SHCalcConstants( 2*vals );
  SHCalc SHCalcu           = SHCalc( 2*vals, SHConsta );
  System sys               = System( const_, mol_ );
  ReExpCoeffsConstants re_exp_consts (sys.get_consts().get_kappa(),
                                      sys.get_lambda(), nvals);
  
  ASolver ASolvTest        = ASolver( nmol, vals, bCalcu, SHCalcu, sys);
  
  EXPECT_NEAR( ASolvTest.get_delta_ni( 0, 1).real(),   0.87554313, preclim);
  EXPECT_NEAR( ASolvTest.get_delta_ni( 0, 5).real(),   0.06832297, preclim);
  
  EXPECT_NEAR( ASolvTest.get_delta_ni( 1, 2).real(),   11.4370663, preclim);
  EXPECT_NEAR( ASolvTest.get_delta_ni( 1, 7).real()/181.9847, 1.0, preclim);
}


TEST_F(ASolverUTest, checkE)
{
  const int vals           = nvals;
  int nmol                 = 2;
  BesselConstants bConsta  = BesselConstants( 2*vals );
  BesselCalc bCalcu        = BesselCalc( 2*vals, bConsta );
  SHCalcConstants SHConsta = SHCalcConstants( 2*vals );
  SHCalc SHCalcu           = SHCalc( 2*vals, SHConsta );
  System sys               = System( const_, mol_ );
  ReExpCoeffsConstants re_exp_consts (sys.get_consts().get_kappa(),
                                      sys.get_lambda(), nvals);
  
  ASolver ASolvTest        = ASolver( nmol, vals, bCalcu, SHCalcu, sys);
  
  EXPECT_NEAR( ASolvTest.get_E_ni( 0, 0, 0).real(), 5.0, preclim);
  EXPECT_NEAR( ASolvTest.get_E_ni( 0, 0, 0).imag(), 0.0, preclim);

  EXPECT_NEAR( ASolvTest.get_E_ni( 0, 5, 0).real(),   -0.15625, preclim);
  EXPECT_NEAR( ASolvTest.get_E_ni( 0, 5, 0).imag(),        0.0, preclim);

  EXPECT_NEAR( ASolvTest.get_E_ni( 0, 6, -5).real(),        0.0, preclim);
  EXPECT_NEAR( ASolvTest.get_E_ni( 0, 6, -5).imag(),        0.0, preclim);

  EXPECT_NEAR( ASolvTest.get_E_ni( 1, 0, 0).real(),-0.4, preclim);
  EXPECT_NEAR( ASolvTest.get_E_ni( 1, 0, 0).imag(), 0.0, preclim);

  EXPECT_NEAR( ASolvTest.get_E_ni( 1, 3, -2).real(), 0.0728481807168, preclim);
  EXPECT_NEAR( ASolvTest.get_E_ni( 1, 3, -2).imag(), 0.6901393135990, preclim);

  EXPECT_NEAR( ASolvTest.get_E_ni( 1, 6, -5).real(), -1.75961914197, preclim);
  EXPECT_NEAR( ASolvTest.get_E_ni( 1, 6, -5).imag(), -1.01329895122, preclim);
}

TEST_F(ASolverUTest, checkSH)
{
  const int vals           = nvals;
  int nmol                 = 2;
  BesselConstants bConsta  = BesselConstants( 2*vals );
  BesselCalc bCalcu        = BesselCalc( 2*vals, bConsta );
  SHCalcConstants SHConsta = SHCalcConstants( 2*vals );
  SHCalc SHCalcu           = SHCalc( 2*vals, SHConsta );
  System sys               = System( const_, mol_ );
  ReExpCoeffsConstants re_exp_consts (sys.get_consts().get_kappa(),
                                      sys.get_lambda(), nvals);
  
  ASolver ASolvTest        = ASolver( nmol, vals, bCalcu, SHCalcu, sys);

  EXPECT_NEAR( ASolvTest.get_SH_ij( 0, 0, 0, 0).real(), 1.0, preclim);
  EXPECT_NEAR( ASolvTest.get_SH_ij( 0, 0, 0, 0).imag(), 0.0, preclim);

  EXPECT_NEAR( ASolvTest.get_SH_ij( 0, 0, 5, 0).real(),-1.0, preclim);
  EXPECT_NEAR( ASolvTest.get_SH_ij( 0, 0, 5, 0).imag(), 0.0, preclim);

  EXPECT_NEAR( ASolvTest.get_SH_ij( 0, 0, 6,-5).real(), 0.0, preclim);
  EXPECT_NEAR( ASolvTest.get_SH_ij( 0, 0, 6,-5).imag(), 0.0, preclim);
  
  EXPECT_NEAR( ASolvTest.get_SH_ij( 1, 0, 0, 0).real(), 1.0, preclim);
  EXPECT_NEAR( ASolvTest.get_SH_ij( 1, 0, 0, 0).imag(), 0.0, preclim);

  EXPECT_NEAR( ASolvTest.get_SH_ij( 1, 0, 3,-2).real(),-0.0522110883, preclim);
  EXPECT_NEAR( ASolvTest.get_SH_ij( 1, 0, 3,-2).imag(), 0.4946303982, preclim);
  
  EXPECT_NEAR( ASolvTest.get_SH_ij( 1, 0, 6, 5).real(), 0.3615486465, preclim);
  EXPECT_NEAR( ASolvTest.get_SH_ij( 1, 0, 6, 5).imag(),-0.2082023636, preclim);

}

TEST_F(ASolverUTest, checkA)
{
  const int vals           = nvals;
  int nmol                 = 2;
  BesselConstants bConsta  = BesselConstants( 2*vals );
  BesselCalc bCalcu        = BesselCalc( 2*vals, bConsta );
  SHCalcConstants SHConsta = SHCalcConstants( 2*vals );
  SHCalc SHCalcu           = SHCalc( 2*vals, SHConsta );
  System sys               = System( const_, mol_ );
  ReExpCoeffsConstants re_exp_consts (sys.get_consts().get_kappa(),
                                      sys.get_lambda(), nvals);
  
  ASolver ASolvTest        = ASolver( nmol, vals, bCalcu, SHCalcu, sys);
  ASolvTest.solve_A(1E-20);
  
  int ct = 0;
  for ( int n = 0; n < 5; n++ )
  {
    for ( int m = 0; m <= n; m++ )
    {
      EXPECT_NEAR( ASolvTest.get_A_ni( 0, n, m).real(), A0[ct],    preclim);
      EXPECT_NEAR( ASolvTest.get_A_ni( 0, n, m).imag(), A0_im[ct], preclim);
      EXPECT_NEAR( ASolvTest.get_A_ni( 1, n, m).real(), A1[ct],    preclim);
      EXPECT_NEAR( ASolvTest.get_A_ni( 1, n, m).imag(), A1_im[ct], preclim);
      ct++;
    }
  }
}

TEST_F(ASolverUTest, checkASing)
{
  const int vals           = nvals;
  int nmol                 = 2;
  BesselConstants bConsta  = BesselConstants( 2*vals );
  BesselCalc bCalcu        = BesselCalc( 2*vals, bConsta );
  SHCalcConstants SHConsta = SHCalcConstants( 2*vals );
  SHCalc SHCalcu           = SHCalc( 2*vals, SHConsta );
  System sys               = System( const_, mol_sing_ );
  ReExpCoeffsConstants re_exp_consts (sys.get_consts().get_kappa(),
                                      sys.get_lambda(), nvals);
  
  ASolver ASolvTest        = ASolver( nmol, vals, bCalcu, SHCalcu, sys);
  ASolvTest.solve_A(1E-20);
  
  int ct = 0;

  for ( int n = 0; n < 5; n++ )
  {
    for ( int m = 0; m <= n; m++ )
    {
      EXPECT_NEAR( ASolvTest.get_A_ni( 0, n, m).real(), A0Sing[ct], preclim);
      EXPECT_NEAR( ASolvTest.get_A_ni( 0, n, m).imag(),        0.0, preclim);
      EXPECT_NEAR( ASolvTest.get_A_ni( 1, n, m).real(), A1Sing[ct], preclim);
      EXPECT_NEAR( ASolvTest.get_A_ni( 1, n, m).imag(),        0.0, preclim);
      ct++;
    }
  }
}

TEST_F(ASolverUTest, checkgradA)
{
  mol_.clear( ); double cg[3] = { 2.0,  2.0, 2.0};
  Pt pos[3] = {Pt(0.0, 0.0, -5.0), Pt(10.0, 7.8, 25.0), Pt(-10.0, 7.8, 25.0)};
  
  for (int molInd = 0; molInd < 3; molInd ++ )
  {
    int M = 1; vector<double> charges(1); vector<Pt> posCharges(1);
    charges[0]    = cg[molInd]; posCharges[0] = pos[molInd];
    
    Molecule molNew( M, cg[molInd], charges, posCharges, pos[molInd]);
    mol_.push_back( molNew );
  }
  
  const int vals           = nvals;
  int nmol                 = 3;
  BesselConstants bConsta  = BesselConstants( 2*vals );
  BesselCalc bCalcu        = BesselCalc( 2*vals, bConsta );
  SHCalcConstants SHConsta = SHCalcConstants( 2*vals );
  SHCalc SHCalcu           = SHCalc( 2*vals, SHConsta );
  System sys               = System( const_, mol_ );
  ReExpCoeffsConstants re_exp_consts (sys.get_consts().get_kappa(),
                                      sys.get_lambda(), nvals);
  
  ASolver ASolvTest        = ASolver( nmol, vals, bCalcu, SHCalcu, sys);
  ASolvTest.solve_A(1E-20);
  ASolvTest.solve_gradA(1E-16);

  int ct = 0;
  for ( int n = 0; n < 3; n++ )
  {
    //for ( int m = 0; m <= n; m++ )
    for ( int m = 0; m < 3; m++ )
    {
      ASolvTest.print_dAidx(n, m, 4);
      ASolvTest.print_dAidy(n, m, 4);
      ASolvTest.print_dAidz(n, m, 4);
//      if (dA00[ct] != 0)
//        EXPECT_NEAR( ASolvTest.get_dAdx_ni( 0, 0, n, m).real()/dA00[ct],
//                     1.0, preclim);
//      if (dA01im[ct] != 0)
//        EXPECT_NEAR( ASolvTest.get_dAdy_ni( 0, 1, n, m).imag()/dA01im[ct],
//                     1.0, preclim);
//      if (dA11[ct] != 0)
//        EXPECT_NEAR( ASolvTest.get_dAdz_ni( 1, 1, n, m).real()/dA11[ct],
//                     1.0, preclim);
//      if (dA10im[ct] != 0)
//        EXPECT_NEAR( ASolvTest.get_dAdx_ni( 1, 0, n, m).imag()/dA10im[ct],
//                     1.0, preclim);
      ct++;
    }
  }
}

//TEST_F(ASolverUTest, checkgradASing)
//{
//  const int vals           = nvals;
//  int nmol                 = 2;
//  BesselConstants bConsta  = BesselConstants( 2*vals );
//  BesselCalc bCalcu        = BesselCalc( 2*vals, bConsta );
//  SHCalcConstants SHConsta = SHCalcConstants( 2*vals );
//  SHCalc SHCalcu           = SHCalc( 2*vals, SHConsta );
//  System sys               = System( const_, mol_sing_ );
//  ReExpCoeffsConstants re_exp_consts (sys.get_consts().get_kappa(),
//                                      sys.get_lambda(), nvals);
//  
//  ASolver ASolvTest        = ASolver( nmol, vals, bCalcu, SHCalcu, sys);
//  ASolvTest.solve_A(1E-7);
//  
//  int ct = 0;
//  
//  for ( int n = 0; n < 5; n++ )
//  {
//    for ( int m = 0; m <= n; m++ )
//    {
//      EXPECT_NEAR( ASolvTest.get_A_ni( 0, n, m).real(), A0Sing[ct], preclim);
//      EXPECT_NEAR( ASolvTest.get_A_ni( 0, n, m).imag(),        0.0, preclim);
//      EXPECT_NEAR( ASolvTest.get_A_ni( 1, n, m).real(), A1Sing[ct], preclim);
//      EXPECT_NEAR( ASolvTest.get_A_ni( 1, n, m).imag(),        0.0, preclim);
//      ct++;
//    }
//  }
//}


#endif
