#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "ctype.h"

#include "mpfr.h"


/*
 *       USAGE: ./copi.exe <n digits of pi>
 * DESCRIPTION: Calculates PI to arbitrary precision using Chudnovsky algorithm and prints result at the end.
 *              Probably won't do well for extremely large values. Also uses the MPFR library to assure
 *              floating points are all correct. This TRUNCATES the value of pi at the end.
 */

int main(int argc, char *argv[]){

    // set rounding type as defined by IEEE 754, round to nearest
    // usually if you change this to another value you will get a different answer!
    // that is why extra digits are calculated at the end and truncated instead
    mpfr_rnd_t rnd = MPFR_RNDN; 

    unsigned int req_ndigits;
    if (argc > 1 && !isdigit(atoi(argv[1]))){
         req_ndigits = atoi(argv[1]); // requested minimum number of digits of precision
    } else {
        printf("USAGE: ./copi.exe <n digits of pi>\n");
        return 1;
    }

    // default value for minimum precision necessary
    unsigned int prec_power = 6; // 2^6 = 64
    mpfr_prec_t min_p = pow(2,prec_power); 

    size_t ndigits = mpfr_get_str_ndigits(10,min_p);
    // requested number of digits plus 10 extra digits to make sure all the carrys are there
    while (req_ndigits+10 > ndigits){
        min_p = pow(2,prec_power++);
        ndigits = mpfr_get_str_ndigits(10,min_p);
    }

    // set to minimum precision necessary to calculate the number digits of pi exactly
    mpfr_set_default_prec(min_p);

     // padding of 10 extra iterations, approx 14 digits of precision per iteration of Chudnovsky algorithm
    unsigned int n_iter = ndigits/(14*min_p) + 10;

    printf("Requested # digits of precision: %u\n",req_ndigits);
    printf("   Actual # digits of precision: %zu\n",ndigits);
    printf("\n");

    // initialize all the variables
    mpfr_t pi;
    mpfr_t term;
    mpfr_t sum;
    mpfr_init(pi);
    mpfr_init(term);
    mpfr_init(sum);
     
    mpfr_t lq;
    mpfr_t lq_a;
    mpfr_init(lq);
    mpfr_init(lq_a);

    mpfr_t xq;
    mpfr_t xq_a;
    mpfr_init(xq);
    mpfr_init(xq_a);

    mpfr_t mq;
    mpfr_init(mq);

    mpfr_t kq;
    mpfr_t kq_a;
    mpfr_t kq_b;
    mpfr_init(kq);
    mpfr_init(kq_a);
    mpfr_init(kq_b);

    mpfr_t c;
    mpfr_t c_a;
    mpfr_t c_b;
    mpfr_init(c);
    mpfr_init(c_a);
    mpfr_init(c_b);

    mpfr_t lq_old;
    mpfr_t xq_old;
    mpfr_t mq_old;
    mpfr_t kq_old;
    mpfr_init(lq_old);
    mpfr_init(xq_old);
    mpfr_init(mq_old);
    mpfr_init(kq_old);

    // starting terms
    mpfr_set_str(lq_old, "13591409", 10, rnd);
    mpfr_set_str(xq_old, "1", 10, rnd);
    mpfr_set_str(mq_old, "1", 10, rnd);
    mpfr_set_str(kq_old, "-6", 10, rnd);

    // setting various coefficients
    mpfr_set_str(c_a, "426880", 10, rnd);
    mpfr_set_str(c_b, "10005", 10, rnd);

    // calculate the coefficient, c
    mpfr_sqrt(c, c_b, rnd);
    mpfr_mul(c, c, c_a, rnd);

    mpfr_set_str(lq_a, "545140134", 10, rnd);

    mpfr_set_str(xq_a, "-262537412640768000", 10, rnd);

    // first term of the sum
    mpfr_set_str(sum, "13591409", 10, rnd); // first term i = 0 

    // actual computation of pi, note the starting index changes to 1
    for(unsigned int i=1; i<n_iter; i++){

        mpfr_add(lq, lq_old, lq_a, rnd);
        mpfr_mul(xq, xq_old, xq_a, rnd);

        mpfr_add_ui(kq, kq_old, 12, rnd);

        mpfr_pow_ui(kq_a, kq, 3, rnd);
        mpfr_mul_ui(kq_b, kq, 16, rnd);
        mpfr_sub(mq, kq_a, kq_b, rnd);
        mpfr_div_ui(mq, mq, i*i*i, rnd);

        mpfr_mul(mq, mq, mq_old, rnd);

        mpfr_set(lq_old,lq, rnd);
        mpfr_set(xq_old,xq, rnd);
        mpfr_set(mq_old,mq, rnd);
        mpfr_set(kq_old,kq, rnd);

        mpfr_mul(term, mq_old, lq_old, rnd);
        mpfr_div(term, term, xq_old, rnd);

        mpfr_add(sum, sum, term, rnd);

    }
    
    // final calculation of pi
    mpfr_div(pi, c, sum, rnd);

    // print result
    printf("****************************\n");
    printf("PI to %u digits\n", req_ndigits);
    printf("****************************\n");

    // getting the value of pi to a string
    char *pi_str = NULL; // string we are actually writing pi to
    char *str = NULL; // just a placeholder variable, should always be NULL no matter what
    mpfr_exp_t exp; // value of the exponent, always equal to 1 for this computation

    pi_str = mpfr_get_str (str, &exp, 10, req_ndigits, pi, MPFR_RNDZ); // extract the string

    // print the string, inserting a period when printing at position exp
    for(unsigned int i=0; i < req_ndigits; i++){
        if (i==exp){
            printf(".");
        }
        printf("%c",pi_str[i]);
    }
    printf("\n");

    // free all variables
    mpfr_clear(pi);
    mpfr_clear(term);
    mpfr_clear(sum);

    mpfr_clear(lq);
    mpfr_clear(lq_a);

    mpfr_clear(xq);
    mpfr_clear(xq_a);

    mpfr_clear(mq);

    mpfr_clear(kq);
    mpfr_clear(kq_a);
    mpfr_clear(kq_b);

    mpfr_clear(c);
    mpfr_clear(c_a);
    mpfr_clear(c_b);

    mpfr_clear(lq_old);
    mpfr_clear(xq_old);
    mpfr_clear(mq_old);
    mpfr_clear(kq_old);

    mpfr_free_str(pi_str);

    return 0;
}
