#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "fixpoint.h"

////////////////////////////////////////////////////////////////////////
// Helper functions
// Note that you can make these "visible" (not static)
// if you want to be able to write unit tests for them
////////////////////////////////////////////////////////////////////////

// TODO: add helper functions

////////////////////////////////////////////////////////////////////////
// Public API functions
////////////////////////////////////////////////////////////////////////

void
fixpoint_init( fixpoint_t *val, uint32_t whole, uint32_t frac, bool negative ) {
  // TODO: implement
  (*val).whole = whole; 
  (*val).frac = frac;
  (*val).negative = negative; 
}

uint32_t
fixpoint_get_whole( const fixpoint_t *val ) {
  // TODO: implement
  return val->whole; 
}

uint32_t
fixpoint_get_frac( const fixpoint_t *val ) {
  // TODO: implement
  return val->frac; 
}

bool
fixpoint_is_negative( const fixpoint_t *val ) {
  // TODO: implement
  return val->negative; 
}

void
fixpoint_negate( fixpoint_t *val ) {
  // TODO: implement
  if (val->negative == 1 || (val->whole == 0 && val-> frac == 0)) {
    val-> negative = 0; 
  } else {
    val -> negative = 1; 
  }
}

int fixpoint_compare_mag(const fixpoint_t *left, const fixpoint_t *right) {
  if (left -> whole > right -> whole) {
        return 1; 
    } else if (right -> whole > left -> whole) {
        return -1; 
    } else {
      if (right -> frac > left -> frac) {
        return -1;  
      } else if (right -> frac < left -> frac){
        return 1;  
      } else {
        return 0; 
      }
    }
}

result_t
fixpoint_add( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
  result_t return_val = RESULT_OK; 
  
  if (left -> negative == right -> negative) {
    uint64_t frac_sum = (uint64_t)left-> frac + right -> frac; 
    uint32_t f_temp = (uint32_t)frac_sum;
    uint32_t carry = frac_sum >> 32; 
    
    uint64_t w_sum = (uint64_t)left -> whole + right -> whole + carry; 
    if (w_sum > UINT32_MAX) {
      return_val = RESULT_OVERFLOW; 
    }
    result -> whole =(uint32_t) w_sum; 
    result -> frac = f_temp;
    result -> negative = left -> negative; 
  } else{ //dif signs
    const fixpoint_t* larger;
    const fixpoint_t* smaller; 
    int left_larger; 
    left_larger = fixpoint_compare_mag(left, right);
    if (left_larger == 1) {
      larger = left; 
      smaller = right; 
    } else { //include == 0
      larger = right; 
      smaller = left; 
    }
    uint32_t takeaway = 0; 
    uint32_t f_temp; 
    if (smaller -> frac > larger -> frac) {
      takeaway = 1; 
      f_temp = (uint32_t)((uint64_t)larger -> frac + ((uint64_t)1<<32) - smaller -> frac); 
    } else {
      f_temp = larger-> frac - smaller -> frac; 
    }
    uint32_t w_temp = larger -> whole - smaller -> whole - takeaway; 
    result -> whole = w_temp; 
    result -> frac = f_temp; 
    result -> negative = larger -> negative;
  }
  return return_val; 
}


result_t
fixpoint_sub( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
  fixpoint_t neg_right = *right; 
  fixpoint_negate(&neg_right); 
  return fixpoint_add(result, left, &neg_right);
}

result_t
fixpoint_mul( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
  result_t return_val = RESULT_OK; 
  result -> negative = (left -> negative != right -> negative); 
  
  __uint128_t L = ((__uint128_t)left -> whole << 32) | left -> frac; 
  __uint128_t R = ((__uint128_t)right -> whole << 32) | right -> frac; 
  
  __uint128_t mult = L* R; 
  __uint128_t S = mult >> 32; 
  
  uint32_t whole = (uint32_t)(S >> 32); 
  uint32_t frac = (uint32_t)(S & 0xFFFFFFFF);

  if (mult>>96 != 0) {
    return_val |= RESULT_OVERFLOW; 
  }
  if ((mult & 0xFFFFFFFF) != 0) {
    return_val |= RESULT_UNDERFLOW; 
  }
  result -> whole = whole; 
  result -> frac = frac; 

  return return_val; 
}

int
fixpoint_compare( const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
  bool return_val = 0; 
  if(left -> negative == right -> negative) {
    return_val = fixpoint_compare_mag(left, right); 
    if (left -> negative == true) {
      return_val = -return_val; 
    }
  }else{
    if (left -> negative) {
      return_val = -1; 
    } else {
      return_val = 1; 
    }
  }
return return_val; 
  
}

void
fixpoint_format_hex( fixpoint_str_t *s, const fixpoint_t *val ) {
  // TODO: implement
  if (val -> negative) {
    snprintf(s->str, FIXPOINT_STR_MAX_SIZE, "-%X.%08X", val->whole, val->frac);
  } else {
    snprintf(s->str, FIXPOINT_STR_MAX_SIZE, "%X.%08X", val->whole, val->frac);
  }
  char *end = s->str + strlen(s->str) - 1;
  while (*end == 0) {
    *end-- = '\0';
  }
  if (*end == '.'){
    *end++ = '0'; 
  }
}

bool
fixpoint_parse_hex( fixpoint_t *val, const fixpoint_str_t *s ) {
  // TODO: implement
  char *newstart = s->str; 
  if ((s->str)[0] == '-') {
    val->negative = true; 
    newstart = s-> str + 1; 
  } else {
    val -> negative = false; 
  }
  //check invalid string:
  //invalid chars
  //too many digits
  int num_before;
  int num_after; 
  sscanf(newstart, "%n.%n", &num_before, &num_after);
  if (num_before > 8 || num_after > 8) {
    return false; 
  }
  int len = strspn(newstart, "ABCDEF0123456789");
  if (len != strlen(newstart)){
    return false; 
  }
  //add 0's 

  //turn hex into fixpoint_t 
  sscanf(newstart, "%d.%d", val->whole, val->frac);
  return true; 
}