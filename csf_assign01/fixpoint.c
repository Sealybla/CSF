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

result_t
fixpoint_add( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
  result_t return_val = RESULT_OK; 
  
  if (left -> negative == right -> negative) {
    uint32_t carry = 0; 
    if (left -> frac > FIXPOINT_STR_MAX_SIZE - right -> frac) {
      return_val = RESULT_UNDERFLOW; 
      carry = 1; 
    } 
    if (left -> whole > FIXPOINT_STR_MAX_SIZE - right -> whole - carry ) {
      return_val = RESULT_OVERFLOW; 
    } 
    
    result -> negative = left -> negative; 
    //add
    uint32_t w_temp = left -> whole + right -> whole + carry; 
    uint32_t f_temp = left -> frac + right -> frac; 
    result -> whole = w_temp; 
    result -> frac = f_temp; 
  } else{ //dif signs
    const fixpoint_t* larger; 
    const fixpoint_t* smaller; 
    int left_larger = fixpoint_compare(left, right);
    if (left_larger == 1) {
      larger = left; 
      smaller = right; 
    } else { //include == 0
      larger = right; 
      smaller = left; 
    }
    uint32_t takeaway = 0; 
    if (smaller -> frac > larger -> frac) {
      takeaway = 1; 
    }
    uint32_t w_temp = larger -> whole - smaller -> whole - takeaway; 
    uint32_t f_temp = larger -> frac - smaller -> frac; 
    result -> whole = w_temp; 
    result -> frac = f_temp; 
    result -> negative = larger -> negative;
  }
  return return_val; 
}


result_t
fixpoint_sub( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
  fixpoint_negate((fixpoint_t *)right);
  return fixpoint_add(result, left, right);
}

result_t
fixpoint_mul( fixpoint_t *result, const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
  result_t return_val = RESULT_OK; 
  if (left -> negative == right -> negative) {
    result -> negative = 0; 
  } else {
    result -> negative = 1; 
  }
  //shift whole up to "tens place"
  __uint128_t PRS = (((__uint128_t)left -> whole)<<32) + ((left -> frac) * right-> frac); 
  __uint128_t TUV = (((__uint128_t)left -> whole)<<32) + ((left -> frac) * right-> whole); 
  __uint128_t SUM = PRS + (TUV << 32); 
  uint32_t left_32 = SUM >> 96; 
  uint32_t right_32 = SUM;
  if (left_32 != 0) {
    return_val = RESULT_OVERFLOW; 
  }
  if (right_32 != 0) {
    return_val = RESULT_UNDERFLOW; 
  }
  if (right_32 != 0 && left_32 != 0) {
    return_val = RESULT_OVERFLOW|RESULT_UNDERFLOW; 
  }
  uint32_t whole_cleft =SUM >> 64; 
  uint32_t frac_cright = SUM >> 32; 
  result -> whole = whole_cleft; 
  result -> frac = frac_cright; 
  return return_val; 
}

int
fixpoint_compare( const fixpoint_t *left, const fixpoint_t *right ) {
  // TODO: implement
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