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
    uint32_t carry = frac_sum >> 32; 
    
    uint64_t w_sum = (uint64_t)left -> whole + right -> whole + carry; 
    if (w_sum > UINT32_MAX) {
      return_val = RESULT_OVERFLOW; 
    }
    result -> whole =(uint32_t) w_sum; 
    result -> frac = (uint32_t)frac_sum;
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
    uint32_t f_temp; 
    uint32_t w_temp; 
    if (smaller -> frac > larger -> frac) {
      f_temp = (uint32_t)((uint64_t)larger -> frac + ((uint64_t)1<<32) - smaller -> frac); 
      w_temp = larger -> whole - smaller -> whole - 1; 
    } else {
      f_temp = larger-> frac - smaller -> frac; 
      w_temp = larger -> whole - smaller -> whole; 
    }
    result -> whole = w_temp; 
    result -> frac = f_temp; 
    result -> negative = larger -> negative;
    if (result-> whole == 0 && result -> frac == 0) {
      result -> negative = false; 
    }
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
  if(iszero(left) || iszero(right)) {
    result -> whole = 0;
    result -> frac = 0;
    result -> negative = false;
    return RESULT_OK;
  }
  uint64_t frac_frac = (uint64_t)left -> frac * right -> frac;
  
  uint64_t whole_whole = (uint64_t)left -> whole * right -> whole;
  //printf("left:%u, right:%u \n", left ->whole, right ->whole);
  //printf("wholeval%lu \n", whole_whole);
  

  uint64_t left_whole_right_frac = ( uint64_t)left -> whole * right -> frac;
  uint64_t right_whole_left_frac = ( uint64_t)right -> whole * left -> frac;

  uint64_t temp_frac = (left_whole_right_frac) + (right_whole_left_frac) + (frac_frac >> 32);
  uint64_t temp_whole = whole_whole + (temp_frac >> 32);

  uint32_t small_frac = (uint32_t) temp_frac;
  uint32_t small_whole = (uint32_t) temp_whole;


  result -> whole = small_whole;
  result -> frac = small_frac;
  //printf("frac: %u \n", small_frac);
  //printf("fracw: %u \n", small_whole);
  //printf("fracwl: %lu \n", temp_whole);
  result -> negative = (left -> negative == right -> negative) ? false : true;

  if ((frac_frac & 0xFFFFFFFF) != 0 && temp_whole > small_whole) {
    return (RESULT_OVERFLOW|RESULT_UNDERFLOW);
  } else if ((frac_frac & 0xFFFFFFFF) != 0) {
    return RESULT_UNDERFLOW;
  } else if (temp_whole > small_whole) {
    return RESULT_OVERFLOW;
  }
  return RESULT_OK;
}


int
fixpoint_compare( const fixpoint_t *left, const fixpoint_t *right ) {
  if (left -> negative && right -> negative) {
    if (left -> whole < right -> whole) {
      return 1;
    } else if (left -> whole > right -> whole) {
      return -1;
    } else {
      if (left -> frac < right -> frac) {
        return 1;
      } else if (left -> frac > right -> frac) {
        return -1;
      } else {
        return 0;
      }
    }
  } else if (!(left -> negative) && !(right -> negative)) {
    if (left -> whole > right -> whole) {
      return 1;
    } else if (left -> whole < right -> whole) {
      return -1;
    } else {
      if (left -> frac > right -> frac) {
        return 1;
      } else if (left -> frac < right -> frac) {
        return -1;
      } else {
        return 0;
      }
    }
  } else{
    return (right -> negative) ? 1 : -1;
  }
}

void u32_to_hex(char *buf, uint32_t v) {
  const char *hex = "0123456789abcdef";
  for(int i = 0; i < 8; ++i) {
    int shift = (7-i)*4;
    buf[i] = hex[(v>>shift) & 0xF];
  }
  buf[8] = '\0';
}

void
fixpoint_format_hex( fixpoint_str_t *s, const fixpoint_t *val ) {
  char val_whole[9];
  if(val->whole == 0) {
    strcpy(val_whole, "0");
  } else {
    snprintf(val_whole, sizeof val_whole, "%x", (unsigned) val-> whole);
  }

  char val_frac[9];
  
  if(val -> frac == 0) {
    strcpy(val_frac, "0");
  } else {
    char frac_process[9];
    u32_to_hex(frac_process, val -> frac);
    int len = 7;
    while(len >= 0 && frac_process[len] == '0') len--;
    memcpy(val_frac, frac_process, len + 1);
    val_frac[len+1] = '\0';
  }

  if( val-> negative && !(val-> whole == 0 && val -> frac== 0)) {
    snprintf(s->str, sizeof s->str, "-%s.%s", val_whole, val_frac);
  } else {
    snprintf(s->str, sizeof s->str, "%s.%s", val_whole, val_frac);
  }
  
}


bool
fixpoint_parse_hex( fixpoint_t *val, const fixpoint_str_t *s ) {
  if(val == NULL ||  s == NULL) return false;
  const char *memo = s->str;
  if(*memo == '\0') return false;
  bool negative = false;
  if(*memo == '-') {
    negative = true;
    memo++;
  }

  uint32_t whole = 0;
  int total_chars = 0;
  uint32_t frac = 0;
  int test = sscanf(memo, "%8x.%8x%n", &whole, &frac, &total_chars);
  
  if (test != 2) return false;

  int whole_chars = 0;
  sscanf(memo, "%8x%n", &whole, &whole_chars);
  if(memo[whole_chars] != '.') return false;

  const char* frac_ptr = memo + whole_chars + 1;
  int frac_chars = 0;
  sscanf(frac_ptr, "%8x%n", &frac, &frac_chars);

  if(whole_chars > 8 || frac_chars > 8 || frac_chars == 0) return false;
  if(memo + whole_chars + 1 + frac_chars != memo + strlen(memo)) return false;
  
  uint64_t big_whole = 0;
  uint64_t big_frac = 0;
  sscanf(memo, "%lx", &big_whole);
  sscanf(frac_ptr, "%lx", &big_frac);
  if(whole != big_whole || frac != big_frac) return false;
  frac = frac<<((8-frac_chars)*4);
  val -> whole = whole;
  val-> frac= frac;
  val-> negative = negative && !(whole && frac);
  return true;
  

}