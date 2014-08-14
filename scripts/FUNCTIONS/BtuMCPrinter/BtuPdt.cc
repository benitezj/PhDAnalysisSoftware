#include "BtuMCPrinter/BtuPdt.hh"

using namespace std;
#include <map>
#include <string>

using std::map;
using std::string;

BtuPdt::BtuPdt()
{
  // Initialize the names of the particles

  
}

BtuPdt::~BtuPdt()
{
  
}

string
BtuPdt::getName(int lund) 
{

  // Stupid way of doing things, but can't think of other solution

  std::map<int,string> lundid;

  lundid[1] = "d";
  lundid[-1] = "anti_d";
  lundid[2] = "u";
  lundid[-2] = "anti_u";
  lundid[3] = "s";
  lundid[-3] = "anti_s";
  lundid[4] = "c";
  lundid[-4] = "anti_c";
  lundid[5] = "b";
  lundid[-5] = "anti_b";
  lundid[6] = "t";
  lundid[-6] = "anti_t";
  lundid[7] = "b_prime";
  lundid[-7] = "anti_b_prime";
  lundid[8] = "t_prime";
  lundid[-8] = "anti_t_prime";
  lundid[21] = "g";
  lundid[11] = "e_minus";
  lundid[-11] = "e_plus";
  lundid[12] = "nu_e";
  lundid[-12] = "anti_nu_e";
  lundid[13] = "mu_minus";
  lundid[-13] = "mu_plus";
  lundid[14] = "nu_mu";
  lundid[-14] = "anti_nu_mu";
  lundid[15] = "tau_minus";
  lundid[-15] = "tau_plus";
  lundid[16] = "nu_tau";
  lundid[-16] = "anti_nu_tau";
  lundid[17] = "L_minus";
  lundid[-17] = "L_plus";
  lundid[18] = "nu_L";
  lundid[-18] = "anti_nu_L";
  lundid[22] = "gamma";
  lundid[10022] = "vpho";
  lundid[20022] = "Cerenkov";
  lundid[23] = "Z0";
  lundid[24] = "W_plus";
  lundid[-24] = "W_minus";
  lundid[25] = "Higgs0";
  lundid[28] = "reggeon";
  lundid[29] = "pomeron";
  lundid[32] = "Z_prime0";
  lundid[33] = "Z_prime_prime0";
  lundid[34] = "W_prime_plus";
  lundid[-34] = "W_prime_minus";
  lundid[35] = "Higgs_prime0";
  lundid[36] = "A0";
  lundid[37] = "Higgs_plus";
  lundid[-37] = "Higgs_minus";
  lundid[40] = "R0";
  lundid[-40] = "anti_R0";
  lundid[41] = "Xu0";
  lundid[42] = "Xu_plus";
  lundid[-42] = "Xu_minus";
  lundid[81] = "specflav";
  lundid[82] = "rndmflav";
  lundid[-82] = "anti_rndmflav";
  lundid[83] = "phasespa";
  lundid[84] = "c_minushadron";
  lundid[-84] = "anti_c_minushadron";
  lundid[85] = "b_minushadron";
  lundid[-85] = "anti_b_minushadron";
  lundid[86] = "t_minushadron";
  lundid[-86] = "anti_t_minushadron";
  lundid[87] = "b_prime_minushadron";
  lundid[-87] = "anti_b_prime_minushadron";
  lundid[88] = "t_prime_minushadron";
  lundid[-88] = "anti_t_prime_minushadron";
  lundid[89] = "Wvirt_plus";
  lundid[-89] = "Wvirt_minus";
  lundid[90] = "diquark";
  lundid[-90] = "anti_diquark";
  lundid[91] = "cluster";
  lundid[92] = "string";
  lundid[93] = "indep";
  lundid[94] = "CMshower";
  lundid[95] = "SPHEaxis";
  lundid[96] = "THRUaxis";
  lundid[97] = "CLUSjet";
  lundid[98] = "CELLjet";
  lundid[99] = "table";
  lundid[500] = "bsbottom";
  lundid[111] = "pi0";
  lundid[211] = "pi_plus";
  lundid[-211] = "pi_minus";
  lundid[210] = "pi_diff_plus";
  lundid[-210] = "pi_diff_minus";
  lundid[20111] = "pi_2S0";
  lundid[20211] = "pi_2S_plus";
  lundid[-20211] = "pi_2S_minus";
  lundid[221] = "eta";
  lundid[20221] = "eta_2S";
  lundid[9020221] = "eta_1405";
  lundid[100331] = "eta_1475";
  lundid[100333] = "phi_1680";
  lundid[331] = "eta_prime";
  lundid[113] = "rho0";
  lundid[110] = "rho_diff0";
  lundid[213] = "rho_plus";
  lundid[-213] = "rho_minus";
  lundid[30113] = "rho_2S0";
  lundid[30213] = "rho_2S_plus";
  lundid[-30213] = "rho_2S_minus";
  lundid[40113] = "rho_3S0";
  lundid[40213] = "rho_3S_plus";
  lundid[-40213] = "rho_3S_minus";
  lundid[223] = "omega";
  lundid[220] = "omega_diff";
  lundid[30223] = "omega_2S";
  lundid[333] = "phi";
  lundid[330] = "phi_diff";
  lundid[10111] = "a_00";
  lundid[10211] = "a_0_plus";
  lundid[-10211] = "a_0_minus";
  lundid[10221] = "f_0";
  lundid[10331] = "f_prime_0";
  lundid[10113] = "b_10";
  lundid[10213] = "b_1_plus";
  lundid[-10213] = "b_1_minus";
  lundid[10223] = "h_1";
  lundid[10333] = "h_prime_1";
  lundid[20113] = "a_10";
  lundid[20213] = "a_1_plus";
  lundid[-20213] = "a_1_minus";
  lundid[20223] = "f_1";
  lundid[20333] = "f_prime_1";
  lundid[115] = "a_20";
  lundid[215] = "a_2_plus";
  lundid[-215] = "a_2_minus";
  lundid[225] = "f_2";
  lundid[50221] = "f_0_1500";
  lundid[335] = "f_prime_2";
  lundid[311] = "K0";
  lundid[-311] = "anti_K0";
  lundid[310] = "K_S0";
  lundid[130] = "K_L0";
  lundid[321] = "K_plus";
  lundid[-321] = "K_minus";
  lundid[313] = "K_star0";
  lundid[-313] = "anti_K_star0";
  lundid[323] = "K_star_plus";
  lundid[-323] = "K_star_minus";
  lundid[10311] = "K_0_star0";
  lundid[-10311] = "anti_K_0_star0";
  lundid[10321] = "K_0_star_plus";
  lundid[-10321] = "K_0_star_minus";
  lundid[10313] = "K_10";
  lundid[-10313] = "anti_K_10";
  lundid[10323] = "K_1_plus";
  lundid[-10323] = "K_1_minus";
  lundid[315] = "K_2_star0";
  lundid[-315] = "anti_K_2_star0";
  lundid[325] = "K_2_star_plus";
  lundid[-325] = "K_2_star_minus";
  lundid[20313] = "K_prime_10";
  lundid[-20313] = "anti_K_prime_10";
  lundid[20323] = "K_prime_1_plus";
  lundid[-20323] = "K_prime_1_minus";
  lundid[100313] = "K_prime_star0";
  lundid[-100313] = "anti_K_prime_star0";
  lundid[100323] = "K_prime_star_plus";
  lundid[-100323] = "K_prime_star_minus";
  lundid[30313] = "K_prime_prime_star0";
  lundid[-30313] = "anti_K_prime_prime_star0";
  lundid[30323] = "K_prime_prime_star_plus";
  lundid[-30323] = "K_prime_prime_star_minus";
  lundid[317] = "K_3_star0";
  lundid[-317] = "anti_K_3_star0";
  lundid[327] = "K_3_star_plus";
  lundid[-327] = "K_3_star_minus";
  lundid[319] = "K_4_star0";
  lundid[-319] = "anti_K_4_star0";
  lundid[329] = "K_4_star_plus";
  lundid[-329] = "K_4_star_minus";
  lundid[411] = "D_plus";
  lundid[-411] = "D_minus";
  lundid[421] = "D0";
  lundid[-421] = "anti_D0";
  lundid[413] = "D_star_plus";
  lundid[-413] = "D_star_minus";
  lundid[423] = "D_star0";
  lundid[-423] = "anti_D_star0";
  lundid[10411] = "D_0_star_plus";
  lundid[-10411] = "D_0_star_minus";
  lundid[10421] = "D_0_star0";
  lundid[-10421] = "anti_D_0_star0";
  lundid[10413] = "D_1_plus";
  lundid[-10413] = "D_1_minus";
  lundid[10423] = "D_10";
  lundid[-10423] = "anti_D_10";
  lundid[415] = "D_2_star_plus";
  lundid[-415] = "D_2_star_minus";
  lundid[425] = "D_2_star0";
  lundid[-425] = "anti_D_2_star0";
  lundid[20413] = "D_prime_1_plus";
  lundid[-20413] = "D_prime_1_minus";
  lundid[20423] = "D_prime_10";
  lundid[-20423] = "anti_D_prime_10";
  lundid[431] = "D_s_plus";
  lundid[-431] = "D_s_minus";
  lundid[433] = "D_s_star_plus";
  lundid[-433] = "D_s_star_minus";
  lundid[10431] = "D_s0_star_plus";
  lundid[-10431] = "D_s0_star_minus";
  lundid[10433] = "D_s1_plus";
  lundid[-10433] = "D_s1_minus";
  lundid[435] = "D_s2_star_plus";
  lundid[-435] = "D_s2_star_minus";
  lundid[20433] = "D_prime_s1_plus";
  lundid[-20433] = "D_prime_s1_minus";
  lundid[30411] = "D_2S_plus";
  lundid[-30411] = "D_2S_minus";
  lundid[30421] = "D_2S0";
  lundid[-30421] = "anti_D_2S0";
  lundid[30413] = "D_star_2S_plus";
  lundid[-30413] = "D_star_2S_minus";
  lundid[30423] = "D_star_2S0";
  lundid[-30423] = "anti_D_star_2S0";
  lundid[511] = "B0";
  lundid[-511] = "anti_B0";
  lundid[521] = "B_plus";
  lundid[-521] = "B_minus";
  lundid[513] = "B_star0";
  lundid[-513] = "anti_B_star0";
  lundid[523] = "B_star_plus";
  lundid[-523] = "B_star_minus";
  lundid[10511] = "B_0_star0";
  lundid[-10511] = "anti_B_0_star0";
  lundid[10521] = "B_0_star_plus";
  lundid[-10521] = "B_0_star_minus";
  lundid[10513] = "B_10";
  lundid[-10513] = "anti_B_10";
  lundid[10523] = "B_1_plus";
  lundid[-10523] = "B_1_minus";
  lundid[515] = "B_2_star0";
  lundid[-515] = "anti_B_2_star0";
  lundid[525] = "B_2_star_plus";
  lundid[-525] = "B_2_star_minus";
  lundid[20513] = "B_prime_10";
  lundid[-20513] = "anti_B_prime_10";
  lundid[20523] = "B_prime_1_plus";
  lundid[-20523] = "B_prime_1_minus";
  lundid[531] = "B_s0";
  lundid[-531] = "anti_B_s0";
  lundid[533] = "B_s_star0";
  lundid[-533] = "anti_B_s_star0";
  lundid[10531] = "B_s0_star0";
  lundid[-10531] = "anti_B_s0_star0";
  lundid[10533] = "B_s10";
  lundid[-10533] = "anti_B_s10";
  lundid[535] = "B_s2_star0";
  lundid[-535] = "anti_B_s2_star0";
  lundid[20533] = "B_prime_s10";
  lundid[-20533] = "anti_B_prime_s10";
  lundid[541] = "B_c_plus";
  lundid[-541] = "B_c_minus";
  lundid[543] = "B_c_star_plus";
  lundid[-543] = "B_c_star_minus";
  lundid[10541] = "B_c0_star_plus";
  lundid[-10541] = "B_c0_star_minus";
  lundid[10543] = "B_c1_plus";
  lundid[-10543] = "B_c1_minus";
  lundid[545] = "B_c2_star_plus";
  lundid[-545] = "B_c2_star_minus";
  lundid[20543] = "B_prime_c1_plus";
  lundid[-20543] = "B_prime_c1_minus";
  lundid[441] = "eta_c";
  lundid[20441] = "eta_c_2S";
  lundid[443] = "J_psi";
  lundid[440] = "psi_diff";
  lundid[30443] = "psi_2S";
  lundid[40443] = "psi_3770";
  lundid[50443] = "psi_4040";
  lundid[60443] = "psi_4160";
  lundid[70443] = "psi_4415";
  lundid[10443] = "h_c";
  lundid[10441] = "chi_c0";
  lundid[20443] = "chi_c1";
  lundid[445] = "chi_c2";
  lundid[551] = "eta_b";
  lundid[20551] = "eta_b_2S";
  lundid[40551] = "eta_b_3S";
  lundid[553] = "Upsilon";
  lundid[30553] = "Upsilon_2S";
  lundid[60553] = "Upsilon_3S";
  lundid[70553] = "Upsilon_4S";
  lundid[80553] = "Upsilon_5S";
  lundid[10553] = "h_b";
  lundid[40553] = "h_b_2P";
  lundid[100553] = "h_b_3P";
  lundid[10551] = "chi_b0";
  lundid[20553] = "chi_b1";
  lundid[555] = "chi_b2";
  lundid[30551] = "chi_b0_2P";
  lundid[50553] = "chi_b1_2P";
  lundid[10555] = "chi_b2_2P";
  lundid[50551] = "chi_b0_3P";
  lundid[110553] = "chi_b1_3P";
  lundid[20555] = "chi_b2_3P";
  lundid[40555] = "eta_b2_1D";
  lundid[60555] = "eta_b2_2D";
  lundid[120553] = "Upsilon_1_1D";
  lundid[30555] = "Upsilon_2_1D";
  lundid[557] = "Upsilon_3_1D";
  lundid[130553] = "Upsilon_1_2D";
  lundid[50555] = "Upsilon_2_2D";
  lundid[10557] = "Upsilon_3_2D";
  lundid[10222] = "sigma_0";
  lundid[1114] = "Delta_minus";
  lundid[-1114] = "anti_Delta_plus";
  lundid[2110] = "n_diffr";
  lundid[-2110] = "anti_n_diffr";
  lundid[2112] = "n0";
  lundid[-2112] = "anti_n0";
  lundid[2114] = "Delta0";
  lundid[-2114] = "anti_Delta0";
  lundid[2210] = "p_diff_plus";
  lundid[-2210] = "anti_p_diff_minus";
  lundid[2212] = "p_plus";
  lundid[-2212] = "anti_p_minus";
  lundid[2214] = "Delta_plus";
  lundid[-2214] = "anti_Delta_minus";
  lundid[2224] = "Delta_plus_plus";
  lundid[-2224] = "anti_Delta_minus_minus";
  lundid[3112] = "Sigma_minus";
  lundid[-3112] = "anti_Sigma_plus";
  lundid[3114] = "Sigma_star_minus";
  lundid[-3114] = "anti_Sigma_star_plus";
  lundid[3122] = "Lambda0";
  lundid[-3122] = "anti_Lambda0";
  lundid[13122] = "Lambda_14050";
  lundid[-13122] = "anti_Lambda_14050";
  lundid[3124] = "Lambda_15200";
  lundid[-3124] = "anti_Lambda_15200";
  lundid[23122] = "Lambda_16000";
  lundid[-23122] = "anti_Lambda_16000";
  lundid[33122] = "Lambda_16700";
  lundid[-33122] = "anti_Lambda_16700";
  lundid[13124] = "Lambda_16900";
  lundid[-13124] = "anti_Lambda_16900";
  lundid[43122] = "Lambda_18000";
  lundid[-43122] = "anti_Lambda_18000";
  lundid[53122] = "Lambda_18100";
  lundid[-53122] = "anti_Lambda_18100";
  lundid[3126] = "Lambda_18200";
  lundid[-3126] = "anti_Lambda_18200";
  lundid[13126] = "Lambda_18300";
  lundid[-13126] = "anti_Lambda_18300";
  lundid[13212] = "Sigma_16600";
  lundid[-13212] = "anti_Sigma_16600";
  lundid[13214] = "Sigma_16700";
  lundid[-13214] = "anti_Sigma_16700";
  lundid[23212] = "Sigma_17500";
  lundid[-23212] = "anti_Sigma_17500";
  lundid[3216] = "Sigma_17750";
  lundid[-3216] = "anti_Sigma_17750";
  lundid[3212] = "Sigma0";
  lundid[-3212] = "anti_Sigma0";
  lundid[3214] = "Sigma_star0";
  lundid[-3214] = "anti_Sigma_star0";
  lundid[3222] = "Sigma_plus";
  lundid[-3222] = "anti_Sigma_minus";
  lundid[3224] = "Sigma_star_plus";
  lundid[-3224] = "anti_Sigma_star_minus";
  lundid[3312] = "Xi_minus";
  lundid[-3312] = "anti_Xi_plus";
  lundid[3314] = "Xi_star_minus";
  lundid[-3314] = "anti_Xi_star_plus";
  lundid[3322] = "Xi0";
  lundid[-3322] = "anti_Xi0";
  lundid[3324] = "Xi_star0";
  lundid[-3324] = "anti_Xi_star0";
  lundid[3334] = "Omega_minus";
  lundid[-3334] = "anti_Omega_plus";
  lundid[14122] = "Lambda_c_2593_plus";
  lundid[-14122] = "anti_Lambda_c_2593_minus";
  lundid[14124] = "Lambda_c_2625_plus";
  lundid[-14124] = "anti_Lambda_c_2625_minus";
  lundid[4112] = "Sigma_c0";
  lundid[-4112] = "anti_Sigma_c0";
  lundid[4114] = "Sigma_c_star0";
  lundid[-4114] = "anti_Sigma_c_star0";
  lundid[4212] = "Sigma_c_plus";
  lundid[-4212] = "anti_Sigma_c_minus";
  lundid[4214] = "Sigma_c_star_plus";
  lundid[-4214] = "anti_Sigma_c_star_minus";
  lundid[4222] = "Sigma_c_plus_plus";
  lundid[-4222] = "anti_Sigma_c_minus_minus";
  lundid[4224] = "Sigma_c_star_plus_plus";
  lundid[-4224] = "anti_Sigma_c_star_minus_minus";
  lundid[4312] = "Xi_prime_c0";
  lundid[-4312] = "anti_Xi_prime_c0";
  lundid[4322] = "Xi_prime_c_plus";
  lundid[-4322] = "anti_Xi_prime_c_minus";
  lundid[4324] = "Xi_c_star_plus";
  lundid[-4324] = "anti_Xi_c_star_minus";
  lundid[4122] = "Lambda_c_plus";
  lundid[-4122] = "anti_Lambda_c_minus";
  lundid[4132] = "Xi_c0";
  lundid[-4132] = "anti_Xi_c0";
  lundid[4232] = "Xi_c_plus";
  lundid[-4232] = "anti_Xi_c_minus";
  lundid[4314] = "Xi_c_star0";
  lundid[-4314] = "anti_Xi_c_star0";
  lundid[24122] = "Lambda_c_2765_plus";
  lundid[-24122] = "anti_Lambda_c_2765_minus";
  lundid[24124] = "Lambda_c_2880_plus";
  lundid[-24124] = "anti_Lambda_c_2880_minus";
  lundid[34122] = "Lambda_c_2925_plus";
  lundid[-34122] = "anti_Lambda_c_2925_minus";
  lundid[34124] = "Lambda_c_2975_plus";
  lundid[-34124] = "anti_Lambda_c_2975_minus";
  lundid[14112] = "Sigma_c_27950";
  lundid[-14112] = "anti_Sigma_c_27950";
  lundid[14114] = "Sigma_c_28500";
  lundid[-14114] = "anti_Sigma_c_28500";
  lundid[14212] = "Sigma_c_2795_plus";
  lundid[-14212] = "anti_Sigma_c_2795_minus";
  lundid[14214] = "Sigma_c_2850_plus";
  lundid[-14214] = "anti_Sigma_c_2850_minus";
  lundid[14222] = "Sigma_c_2795_plus_plus";
  lundid[-14222] = "anti_Sigma_c_2795_minus_minus";
  lundid[14224] = "Sigma_c_2850_plus_plus";
  lundid[-14224] = "anti_Sigma_c_2850_minus_minus";
  lundid[14132] = "Xi_c_27900";
  lundid[-14132] = "anti_Xi_c_27900";
  lundid[14232] = "Xi_c_2790_plus";
  lundid[-14232] = "anti_Xi_c_2790_minus";
  lundid[14134] = "Xi_c_28150";
  lundid[-14134] = "anti_Xi_c_28150";
  lundid[14234] = "Xi_c_2815_plus";
  lundid[-14234] = "anti_Xi_c_2815_minus";
  lundid[14312] = "Xi_c_29000";
  lundid[-14312] = "anti_Xi_c_29000";
  lundid[14322] = "Xi_c_2900_plus";
  lundid[-14322] = "anti_Xi_c_2900_minus";
  lundid[14314] = "Xi_c_29500";
  lundid[-14314] = "anti_Xi_c_29500";
  lundid[14324] = "Xi_c_2950_plus";
  lundid[-14324] = "anti_Xi_c_2950_minus";
  lundid[4412] = "Xi_cc_plus";
  lundid[-4412] = "anti_Xi_cc_minus";
  lundid[4422] = "Xi_cc_plus_plus";
  lundid[-4422] = "anti_Xi_cc_minus_minus";
  lundid[4432] = "Omega_cc_plus";
  lundid[-4432] = "anti_Omega_cc_minus";
  lundid[4414] = "Xi_cc_star_plus";
  lundid[-4414] = "anti_Xi_cc_star_minus";
  lundid[4424] = "Xi_cc_star_plus_plus";
  lundid[-4424] = "anti_Xi_cc_star_minus_minus";
  lundid[4434] = "Omega_cc_star_plus";
  lundid[-4434] = "anti_Omega_cc_star_minus";
  lundid[4332] = "Omega_c0";
  lundid[-4332] = "anti_Omega_c0";
  lundid[4334] = "Omega_c_star0";
  lundid[-4334] = "anti_Omega_c_star0";
  lundid[14332] = "Omega_c_28500";
  lundid[-14332] = "anti_Omega_c_28500";
  lundid[14334] = "Omega_c_29000";
  lundid[-14334] = "anti_Omega_c_29000";
  lundid[5112] = "Sigma_b_minus";
  lundid[-5112] = "anti_Sigma_b_plus";
  lundid[5114] = "Sigma_b_star_minus";
  lundid[-5114] = "anti_Sigma_b_star_plus";
  lundid[5122] = "Lambda_b0";
  lundid[-5122] = "anti_Lambda_b0";
  lundid[5132] = "Xi_b_minus";
  lundid[-5132] = "anti_Xi_b_plus";
  lundid[5212] = "Sigma_b0";
  lundid[-5212] = "anti_Sigma_b0";
  lundid[5214] = "Sigma_b_star0";
  lundid[-5214] = "anti_Sigma_b_star0";
  lundid[5222] = "Sigma_b_plus";
  lundid[-5222] = "anti_Sigma_b_minus";
  lundid[5224] = "Sigma_b_star_plus";
  lundid[-5224] = "anti_Sigma_b_star_minus";
  lundid[5232] = "Xi_b0";
  lundid[-5232] = "anti_Xi_b0";
  lundid[5312] = "Xi_prime_b_minus";
  lundid[-5312] = "anti_Xi_prime_b_plus";
  lundid[5314] = "Xi_b_star_minus";
  lundid[-5314] = "anti_Xi_b_star_plus";
  lundid[5322] = "Xi_prime_b0";
  lundid[-5322] = "anti_Xi_prime_b0";
  lundid[5324] = "Xi_b_star0";
  lundid[-5324] = "anti_Xi_b_star0";
  lundid[5332] = "Omega_b_minus";
  lundid[-5332] = "anti_Omega_b_plus";
  lundid[5334] = "Omega_b_star_minus";
  lundid[-5334] = "anti_Omega_b_star_plus";
  lundid[1101] = "dd_0";
  lundid[-1101] = "anti_dd_0";
  lundid[2101] = "ud_0";
  lundid[-2101] = "anti_ud_0";
  lundid[2201] = "uu_0";
  lundid[-2201] = "anti_uu_0";
  lundid[3101] = "sd_0";
  lundid[-3101] = "anti_sd_0";
  lundid[3201] = "su_0";
  lundid[-3201] = "anti_su_0";
  lundid[3301] = "ss_0";
  lundid[-3301] = "anti_ss_0";
  lundid[4101] = "cd_0";
  lundid[-4101] = "anti_cd_0";
  lundid[4201] = "cu_0";
  lundid[-4201] = "anti_cu_0";
  lundid[4301] = "cs_0";
  lundid[-4301] = "anti_cs_0";
  lundid[4401] = "cc_0";
  lundid[-4401] = "anti_cc_0";
  lundid[5101] = "bd_0";
  lundid[-5101] = "anti_bd_0";
  lundid[5201] = "bu_0";
  lundid[-5201] = "anti_bu_0";
  lundid[5301] = "bs_0";
  lundid[-5301] = "anti_bs_0";
  lundid[5401] = "bc_0";
  lundid[-5401] = "anti_bc_0";
  lundid[5501] = "bb_0";
  lundid[-5501] = "anti_bb_0";
  lundid[1103] = "dd_1";
  lundid[-1103] = "anti_dd_1";
  lundid[2103] = "ud_1";
  lundid[-2103] = "anti_ud_1";
  lundid[2203] = "uu_1";
  lundid[-2203] = "anti_uu_1";
  lundid[3103] = "sd_1";
  lundid[-3103] = "anti_sd_1";
  lundid[3203] = "su_1";
  lundid[-3203] = "anti_su_1";
  lundid[3303] = "ss_1";
  lundid[-3303] = "anti_ss_1";
  lundid[4103] = "cd_1";
  lundid[-4103] = "anti_cd_1";
  lundid[4203] = "cu_1";
  lundid[-4203] = "anti_cu_1";
  lundid[4303] = "cs_1";
  lundid[-4303] = "anti_cs_1";
  lundid[4403] = "cc_1";
  lundid[-4403] = "anti_cc_1";
  lundid[5103] = "bd_1";
  lundid[-5103] = "anti_bd_1";
  lundid[5203] = "bu_1";
  lundid[-5203] = "anti_bu_1";
  lundid[5303] = "bs_1";
  lundid[-5303] = "anti_bs_1";
  lundid[5403] = "bc_1";
  lundid[-5403] = "anti_bc_1";
  lundid[5503] = "bb_1";
  lundid[-5503] = "anti_bb_1";
  lundid[1011] = "deuteron";
  lundid[-1011] = "anti_deuteron";
  lundid[1021] = "tritium";
  lundid[-1021] = "anti_tritium";
  lundid[1012] = "He3";
  lundid[-1012] = "anti_He3";
  lundid[1022] = "alpha";
  lundid[-1022] = "anti_alpha";
  lundid[100] = "geantino";
  lundid[101] = "chargedgeantino";
  lundid[30343] = "Xsd";
  lundid[-30343] = "anti_Xsd";
  lundid[30353] = "Xsu";
  lundid[-30353] = "anti_Xsu";
  lundid[30373] = "Xdd";
  lundid[-30373] = "anti_Xdd";
  lundid[30383] = "Xdu";
  lundid[-30383] = "anti_Xdu";
  lundid[30363] = "Xss";
  lundid[-30363] = "anti_Xss";
  lundid[51] = "dummy00_1";
  lundid[52] = "dummy10_1";
  lundid[53] = "dummy01_1";
  lundid[54] = "dummy11_1";
  lundid[-51] = "anti_dummy00_1";
  lundid[-52] = "anti_dummy10_1";
  lundid[-53] = "anti_dummy01_1";
  lundid[-54] = "anti_dummy11_1";
  lundid[55] = "dummy00_2";
  lundid[56] = "dummy10_2";
  lundid[57] = "dummy01_2";
  lundid[58] = "dummy11_2";
  lundid[-55] = "anti_dummy00_2";
  lundid[-56] = "anti_dummy10_2";
  lundid[-57] = "anti_dummy01_2";
  lundid[-58] = "anti_dummy11_2";
  lundid[0] = "null";



//    printf("%s\n",name[index]);

 string pdtname = "unknown";

map<int,string>::iterator position =  lundid.find( lund );

if ( position != lundid.end() ) {
  pdtname = position->second;
 }
  return pdtname;
}
