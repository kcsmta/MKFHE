#include <iostream>
#include <cassert>
#include <time.h>
#include <cstdint>
#include <stdexcept>
#include <chrono>
#include <limits.h>

#include <NTL/ZZX.h>

#include "FINAL.h"

using namespace std;
using namespace NTL;

int main()
{
    // ============================
    // ======== Single-key ========
    // ============================
    // // Input bits
    // int b1 = 1;
    // int b2 = 1;

    // // LWE encryption base scheme 
    // // and ciphertexts
    // // SchemeLWE s;
    // // Ctxt_LWE ct1, ct2, ct_and;
    // SchemeNTRU s;
    // Ctxt_NTRU ct1, ct2, ct_nand;

    // // Encryption of bits
    // s.encrypt(ct1, b1);
    // s.encrypt(ct2, b2);

    // // Computes NAND
    // s.nand_gate(ct_nand, ct1, ct2);
    // assert(s.decrypt(ct_nand) == 0);

    // ============================
    // ========= MK-MNTRU =========
    // ============================
    // Input bits
    int b1 = 1;
    int b2 = 1;
    int n_party = parNTRU.n_party;

    // LWE encryption base scheme 
    // and ciphertexts
    vector<SchemeNTRU> s(n_party);
    vector<Ctxt_NTRU> ct1(n_party), ct2(n_party), ct_nand(n_party);

    vector<double> partial_decrypt(n_party);
    for(int i=0; i<n_party; i++){
        Ctxt_NTRU temp1;
        s[i].encrypt(temp1, b1);
        ct1[i] = temp1;

        Ctxt_NTRU temp2;
        s[i].encrypt(temp2, b2);
        ct2[i] = temp2;

        // Computes NAND [NAND->bootstrapping(->key_switching)] in each coordinate
        Ctxt_NTRU temp_nand;
        s[i].nand_gate(temp_nand, ct1[i], ct2[i]);
        ct_nand[i] = temp_nand;
        partial_decrypt[i] = s[i].decrypt(ct_nand[i]);
    }

    // Decryption
    int output = 0;
    for(int i=0;i<n_party;i++){
        output += partial_decrypt[i];
    }
    output = parNTRU.mod_q_base(output);

    // // FINAL version
    // output = int(round(double(output)/double(parNTRU.q_base)*Param::t));
    // cout << output << endl;

    // A The Anh version
    float out = abs(1.0*output/(parNTRU.q_base/2));
    cout << out << endl;
    if (out < 0.5)
        cout << 0 << endl;
    else
        cout << 1 << endl;

    // // // ============================
    // // // ========== MK-LWE ==========
    // // // ============================
    // // Input bits
    // int b1 = 0;
    // int b2 = 1;
    // int n_party = parLWE.n_party;

    // // LWE encryption base scheme 
    // // and ciphertexts
    // vector<SchemeLWE> s(n_party);
    // vector<Ctxt_LWE> ct1(n_party), ct2(n_party), ct_nand(n_party);

    // vector<double> partial_decrypt(n_party);
    // for(int i=0; i<n_party; i++){
    //     Ctxt_LWE temp1;
    //     s[i].encrypt(temp1, b1);
    //     ct1[i] = temp1;

    //     Ctxt_LWE temp2;
    //     s[i].encrypt(temp2, b2);
    //     ct2[i] = temp2;

    //     // Computes NAND [NAND->bootstrapping(->key_switching)] in each coordinate
    //     Ctxt_LWE temp_nand;
    //     s[i].nand_gate(temp_nand, temp1, temp2);
    //     ct_nand[i] = temp_nand;
    //     partial_decrypt[i] = s[i].decrypt(ct_nand[i]);
    // }

    // // Decryption
    // int output = 0;
    // for(int i=0;i<n_party;i++){
    //     output += partial_decrypt[i];
    // }
    // output = parNTRU.mod_q_base(output);

    // // // FINAL version
    // // output = int(round(double(output)/double(parNTRU.q_base)*Param::t));
    // // cout << output << endl;

    // // OR
    // // A The Anh version
    // float out = abs(1.0*output/parNTRU.q_base/4);
    // cout << out << endl;
    // if (out < 0.5)
    //     cout << 0 << endl;
    // else
    //     cout << 1 << endl;

    return 0;
}
