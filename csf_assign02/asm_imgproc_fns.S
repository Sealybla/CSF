/*
 * x86-64 assembly language implementations of functions
 */

	.section .text

/* Offsets of struct Image fields */
#define IMAGE_WIDTH_OFFSET   0
#define IMAGE_HEIGHT_OFFSET  4
#define IMAGE_DATA_OFFSET    8

/*
 * TODO: define your helper functions here.
 * Don't forget to use the .globl directive to make
 * them visible to the test code.
 */

 .globl get_r
 get_r:
	movl %edi, %eax
	shrl $24, %eax
	andl $0xFF, %eax
	ret


.globl get_g
 get_g: 
 	movl %edi, %eax
	shrl $16, %eax
	andl $0xFF, %eax
	ret

.globl get_b
 get_b: 
	movl %edi, %eax
	shrl $8, %eax
	andl $0xFF, %eax
	ret

.globl abs_diff
abs_diff:
    movl %edi, %eax       # a
    subl %esi, %eax       # eax = a - b
    movl %eax, %edx
    negl %edx
    cmovl %edx, %eax      # if eax < 0, replace with -eax
    ret


.globl get_largest_abs_diff
get_largest_abs_diff:
    # params: edi=r, esi=g, edx=b, ecx=nr, r8d=ng, r9d=nb
	# new params: rax = return val, edx(new)=nrr, ecx(new)=ngg, r8d(new)=nbb (callee saved)
	# other: eax (output of helper), r10d temporary
	#get nrr
	/*more callee saved registers*/

	
	pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
	/*move original to new callee saved registers, other than r9 which stays*/
	movl %edi, %ebx 	#rs
	movl %esi, %r12d 	#g
	movl %edx, %r13d	#b
	movl %ecx, %r14d 	#nr
	movl %r8d, %r15d 	#ng
	#note: r9d is nb
	/*calculate nrr*/
	movl %ebx, %edi
	movl %r14d, %esi 
	call abs_diff
	movl %eax, %edx
	/*calculate ngg*/
	movl %r12d, %edi
	movl %r15d, %esi
	call abs_diff
	movl %eax, %ecx
	/*calculate nbb*/
	movl %r13d, %edi
	movl %r9d, %esi
	call abs_diff
	movl %eax, %r8d
	/*not (nrr >= ngg) --> jump to else*/
	cmpl %edx, %ecx
	jg .else
.if: 
	/*not (nrr >= nbb) --> jump to if else*/
	cmpl %edx, %r8d
	jg .if_else
	/*return (int)nr - (int)r; */
	movl %r14d, %rax
	subl %ebx, %rax
	jmp .return_val
.if_else: 
	/*return (int)nb - (int)b; */
	movl %r9d, %rax
	subl %r13d, %rax
	jmp .return_val
.else:
	/* not (ngg >= nbb) --> jump to else_else*/
	cmpl %ecx, %r8d
	jg .else_else
	/*return (int)ng - (int)g; */
	movl %r15d, %rax
	subl %r12d %rax
	jmp .return_val
.else_else:
	/*return (int)nb - (int)b;*/
	movl %r9d, %rax
	subl %r13d, %rax
	jmp .return_val
.return_val:
	popq %r15
    popq %r14  
    popq %r13
    popq %r12
    popq %rbx
    ret


/*
 * Definitions of image transformation functions
 */

/*
 *  Transform the color component values in each input pixel
 *  by applying the bitwise complement operation. I.e., each bit
 *  in the color component information should be inverted
 *  (1 becomes 0, 0 becomes 1.) The alpha value of each pixel should
 *  be left unchanged.
 * 
 *  Parameters:
 *  %rdi - pointer to the input Image
 *  %rsi - pointer to the output Image (in which the
 *         transformed pixels should be stored)
 *	%rcx - index
 * 	%rdx - data 
 *	%r8 - output data
 *	%r11d- total # pixels
 *	Temporaries: %eax, %r9d, %r10d
 */ 
	.globl imgproc_complement
imgproc_complement:
	/* TODO: implement */
	/*compute total pixels into r11d */
	movl IMAGE_WIDTH_OFFSET(%rdi), %r11d /*width*/
	movl IMAGE_HEIGHT_OFFSET(%rdi), %eax /*height*/
	imul %eax, %r11d /*multiply width by height, store into eax*/

	/*data init*/
	movq IMAGE_DATA_OFFSET(%rdi), %rdx/*input data*/
	movq IMAGE_DATA_OFFSET(%rsi), %r8 /*output data*/

	movq $0, %rcx  	/*initialize index = 0*/

/*loop across rcx */
.loop: 
	/*get and save pixel data: alpha, red, green, blue*/
	movl (%rdx, %rcx, 4), %edi	/*pixel (data location)*/
	call get_r			/*get red pixel value*/
	movl %eax, %r10d	/*move red value into r10 temporarily */
	notl %r10d			/*flips bits*/
	andl $0xFF, %r10d 	/*make sure only bottom 8 bits*/
	shll $24, %r10d		/*move to correct*/

	call get_g			/*get green pixel value*/
	movl %eax, %r9d 	/*move green value into r9 temporarily */
	notl %r9d			/*flips bits*/
	andl $0xFF, %r9d	/*make sure only bottom 8 bits*/
	shll $16, %r9d		/*move to correct place*/
	orl %r9d, %r10d		/*add to r10d*/

	call get_b			/*get blue pixel value*/
	movl %eax, %r9d 	/*move blue value into r9 temporarily (overwrite) */
	notl %r9d			/*flips bits*/
	andl $0xFF, %r9d	/*make sure only bottom 8 bits*/
	shll $8, %r9d		/*move to correct place*/
	orl %r9d, %r10d		/*add to r10d*/

	movl %edi, %r9d 	/*move pixel into r9 temporarily (overwrite) */
	andl $0xFF, %r9d	/*make sure only bottom 8 bits*/
	orl %r9d, %r10d		/*add to r10d*/

	/*input this data into new pixel*/
	movl %r10d, (%r8, %rcx, 4)			/*moves complemented data into correct index (ecx) */

	incq %rcx	/*increase index*/
	cmp %r11d, %ecx	/*compare indices and break rcx is greater*/
	jl .loop	/*loop again if rcx < r11d*/
	ret

/*
 *  Transform the input image by swapping the row and column
 *  of each source pixel when copying it to the output image.
 *  E.g., a pixel at row i and column j of the input image
 *  should be copied to row j and column i of the output image.
 *  Note that this transformation can only be applied to square
 *  images (where the width and height are identical.)
 * 
 *  Parameters:
 *  %rdi - pointer to the input Image
 *  %rsi - pointer to the output Image (in which the
 *                    transformed pixels should be stored)
 *  %rbx - pointer to the input pixel data array
 *	%r12 - pointer to the output pixel data array
 * 	%r13 - used as row index i in outer loop
 *  @return 1 if the transformation succeeded, or 0 if the
 *          transformation can't be applied because the image
 *          width and height are not the same
 */
	.globl imgproc_transpose
imgproc_transpose:
	/* TODO: implement */
	pushq %rbx /* pointer to the input pixel data array */
	pushq %r12 /* pointer to the output pixel data array */
	pushq %r13 /* row index i in outer loop */

	movl IMAGE_WIDTH_OFFSET(%rdi), %edx /*width*/
	movl IMAGE_HEIGHT_OFFSET(%rdi), %eax /*height*/
	
	cmp	%edx , %eax
	je	.success /* jump equal - run regularly */
	jmp	.fail /* jump to the failed section to return 0 */

/* case of when width == height */
.success:
	/* copying the input dims into the ouput dims */
	movl	%edx, (%rsi) /* output -> width to input */
	movl	%eax, IMAGE_HEIGHT_OFFSET(%rsi) /* output -> height to input */

	/* pointers to input + output data after the width and heigh dims */
	movq	IMAGE_DATA_OFFSET(%rdi), %rbx /* rbx is the input data */
	movq	IMAGE_DATA_OFFSET(%rsi), %r12 /* r12 is the output data */
	movl $0, %r13d /* set outer loop for %r13d = 0 */
/* outer loop */
.row_loop:
	cmp	%edx, %r13d /* loop while %r13d < width */
	jge	.done /* if loop is done or %r13d >= width == %edx then jump to .done */

	movl $0, %ecx /* set inner loop for %ecx = 0 */
/* inner loop */
.col_loop:
	cmp	%eax, %ecx
	jge .next_row /* if loop is done or %ecx >= height == %eax then jump to .next_row */

	movl	%r13d, %r8d 	/* copy %r13d to %r8d */
	imull	%eax, %r8d 		/* multiply %r8d by height */
	addl	%ecx, %r8d  	/* %r8d = i * height + %ecx */
	movslq	%r8d, %r8		/* extend the sign to 64 bits */
	movl	(%rbx, %r8, 4), %r9d /* r9d = %rbx + %r8 * 4  - 4 is the scale */

	movl	%ecx, %r10d 	/* copy %ecx to %r10d */
    imull	%edx, %r10d 	/* multiply %r8d by width */ 
    addl	%r13d, %r10d  	/* %r10d = i * width + %r13d */
    movslq	%r10d, %r10 	/* extend the sign to 64 bits */
    movl	%r9d, (%r12,%r10,4) /* %r12 + %r10 * 4  = %r9d*/

    incl 	%ecx    /* increment the inner counter */       
    jmp 	.col_loop /* run next col loop until the counter reaches height */

/* to run next row */
.next_row:
    incl	 %r13d  	/* add one to row count */            
    jmp 	.row_loop 	/* run the row loop */

/* successful finish */
.done:
    movl 	$1, %eax   /* return value 0 */        
    jmp		.cleanup  /* clean + return */

/* case of width not equal to height */
.fail:
	movl 	$0, %eax  /* return value 0 */    
    jmp		.cleanup  /* clean + return */

/* cleaning + return */
.cleanup:
	popq  	%r13 /* cleanup %r13 */
    popq	%r12 /* cleanup %r12 */
    popq	%rbx /* cleanup %rbx */
    ret 		/* return */
/*
 *  Transform the input image by copying only those pixels that are
 *  within an ellipse centered within the bounds of the image.
 *  Pixels not in the ellipse should be left unmodified, which will
 *  make them opaque black.
 * 
 *  Let w represent the width of the image and h represent the
 *  height of the image. Let a=floor(w/2) and b=floor(h/2).
 *  Consider the pixel at row b and column a is being at the
 *  center of the image. When considering whether a specific pixel
 *  is in the ellipse, x is the horizontal distance to the center
 *  of the image and y is the vertical distance to the center of
 *  the image. The pixel at the coordinates described by x and y
 *  is in the ellipse if the following inequality is true:
 * 
 *    floor( (10000*x*x) / (a*a) ) + floor( (10000*y*y) / (b*b) ) <= 10000
 * 
 *  Parameters:
 *  %rdi - pointer to the input Image
 *  %rsi - pointer to the output Image (in which the
 *                    transformed pixels should be stored)
 *	%r15 - width of the image
 *  %r13 - height of the image
 *	%r14 - output data
 *	%r11 - input data
 *	
 */
	.globl imgproc_ellipse
imgproc_ellipse:
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15


	/* calculate */

	movl	IMAGE_WIDTH_OFFSET(%rdi), %r12d
	shrl	$1, %r12d
	
	movl	IMAGE_HEIGHT_OFFSET(%rdi), %r13d
	shrl	$1, %r13d

	cmpl $0, %r12d
	je .done_two
	cmpl $0, %r13d  
	je .done_two

	/* copy to output width and length */
	movl IMAGE_WIDTH_OFFSET(%rdi), %eax
	movl %eax, IMAGE_WIDTH_OFFSET(%rsi)   
	movl IMAGE_HEIGHT_OFFSET(%rdi), %eax
	movl %eax, IMAGE_HEIGHT_OFFSET(%rsi) 

	/* pointers to input + output data after the width and heigh dims */
	movq	IMAGE_DATA_OFFSET(%rdi), %r11 /* r11 is the input data */
	movq	IMAGE_DATA_OFFSET(%rsi), %r14 /* r14 is the output data */

	movl	$0, %r10d /* set outer loop for %r10d = 0 */

/* outer loop */
.row_loop_two:
	cmp	IMAGE_HEIGHT_OFFSET(%rdi), %r10d /* loop while %edx< height */
	jge	.done_two /* if loop is done or %edx >= width == %edx then jump to .done */

	movl $0, %ecx /* set inner loop for %ecx = 0 */
/* inner loop */
.col_loop_two:
	cmp	IMAGE_WIDTH_OFFSET(%rdi), %ecx
	jge .next_row_two /* if loop is done or %ecx >= width == %eax then jump to .next_row */

	/* calculating (10000*y*y) */
	movl	%r10d, %r9d 	/* copy (height) %ecx to %r9d */
	subl	%r13d, %r9d
	imull	%r9d, %r9d	/* square %r9d */
	imull	$10000, %r9d	/* multiply by 10000 */

	/* division */
	movl	%r9d, %eax /* copy the quotient */
	movl	$0, %edx /* empty edx to be safe*/
	movl	%r13d, %r15d
	imull	%r15d, %r15d
	divl	%r15d
	movl	%eax, %r9d

	/* calculating (10000*x*x) */
	movl	%ecx, %r8d /* copy the width counter to r8d */
	subl	%r12d, %r8d
	imull	%r8d, %r8d	/* square %r8d */
	imull	$10000, %r8d /* multiply r8d by 10000 */

	/* division */
	movl	%r8d, %eax /* copy the quotient */
	movl	$0, %edx /* empty edx to be safe*/
	movl	%r12d, %r15d
	imull	%r15d, %r15d
	divl	%r15d
	movl	%eax, %r8d

	addl	%r8d, %r9d /* floor((10000*x*x)/a) + floor((10000*y*y)/a) */
	cmp 	$10000, %r9d
	jle 	.in_ellipse

	/* set values outside of ellipse to black */
	movl	%r10d, %r9d 	/* copy %ecx to %r9d */
    imull	IMAGE_WIDTH_OFFSET(%rdi), %r9d 	/* multiply %r8d by width */ 
    addl	%ecx, %r9d  	/* %r9d = ecx * width + %r10d */
    movslq	%r9d, %r9 	/* extend the sign to 64 bits */
    movl	$0xFF, (%r14,%r9,4) /* copy to the output*/

    incl 	%ecx    /* increment the inner counter */       
    jmp 	.col_loop_two /* run next col loop until the counter reaches height */

/* to run next row */
.next_row_two:
    incl	 %r10d  	/* add one to row count */            
    jmp 	.row_loop_two 	/* run the row loop */

/* case of in ellipse and we copy the points directly */
.in_ellipse:
	movl	%r10d, %r9d 	/* copy %ecx to %r9d */
    imull	IMAGE_WIDTH_OFFSET(%rdi), %r9d 	/* multiply %r8d by width */ 
    addl	%ecx, %r9d  	/* %r9d = ecx * width + %r10d */
    movslq	%r9d, %r9 	/* extend the sign to 64 bits */
    movl (%r11, %r9, 4), %eax      /* Load from input into register */
	movl %eax, (%r14,%r9,4)        /* Store from register to output */
	incl 	%ecx    /* increment the inner counter */       
    jmp 	.col_loop_two /* run next col loop until the counter reaches height */

/* successful finish */
.done_two:
	movl $1, %eax /* set 1 as return value */
	popq %r15
	popq %r14
	popq %r13
	popq %r12

    ret 		/* return */

/*
 *  Transform the input image using an "emboss" effect. The pixels
 *  of the source image are transformed as follows.
 * 
 *  The top row and left column of pixels are transformed so that their
 *  red, green, and blue color component values are all set to 128,
 *  and their alpha values are not modified.
 * 
 *  For all other pixels, we consider the pixel's color component
 *  values r, g, and b, and also the pixel's upper-left neighbor's
 *  color component values nr, ng, and nb. In comparing the color
 *  component values of the pixel and its upper-left neighbor,
 *  we consider the differences (nr-r), (ng-g), and (nb-b).
 *  Whichever of these differences has the largest absolute value
 *  we refer to as diff. (Note that in the case that more than one
 *  difference has the same absolute value, the red difference has
 *  priority over green and blue, and the green difference has priority
 *  over blue.)
 * 
 *  From the value diff, compute the value gray as 128 + diff.
 *  However, gray should be clamped so that it is in the range
 *  0..255. I.e., if it's negative, it should become 0, and if
 *  it is greater than 255, it should become 255.
 * 
 *  For all pixels not in the top or left row, the pixel's red, green,
 *  and blue color component values should be set to gray, and the
 *  alpha value should be left unmodified.
 * 
 *  Parameters:
 *  %rdi - pointer to the input Image
 *  %rsi - pointer to the output Image (in which the
 *         transformed pixels should be stored)
 *	Variables: 
 *	%r14 - index
 * 	%r12 - data 
 *	%r13 - output data
 *	Temporaries: %rax, %rcx, %rdx, %rdi, %rsi %r8, %r9, %r10, %r11
 */
	.globl imgproc_emboss
imgproc_emboss:
	/* TODO: implement */
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	movq %rdi, %r12		#input data 
	movq %rsi, %r13		#outputs data
	movq $0, %r14 		#initialize index
.emboss_loop:
	/*check if first row*/
	cmpl %r14d, IMAGE_WIDTH_OFFSET(%r12)	#compare index with width
	jl .if_gray			#jump to (turn pixel gray func) if first row

	/*check if r14d is first column*/

	movl %r14d, %eax		#moves current index into eax for use (low 32 bits)
	xor %edx, %edx		#set edx = 0 (high 32 bits)
	movl IMAGE_WIDTH_OFFSET(%r12), %r10d
	divl %r10d			#divide edx:eax by width, quotient goes to eax, remainder goes to edx

	testl %edx, %edx	#check edx is 0 
	jz .if_gray			#jump to (turn pixel gray function) if first col

	/*not first row/col*/
	# params: edi=r (temp rbx), esi=g, edx=b, ecx=nr, r8d=ng, r9d=nb (input into get_largest_abs_diff), output rax
	# getrgb: input - rdi, ouptut-rax 
	movl IMAGE_DATA_OFFSET(%r12), %r9d	#move data into r9d (will override later)
	movl (%r9d, %r14d, 4), %edi			#get current pixel
	/*get r*/
	call get_r
	movl %eax, %rbx
	/*get g*/
	call get_g
	movl %eax, %esi
	/*get b*/
	call get_b
	movl %eax, %edx

	/*top left corner index into r8d (will override later)*/
	movl %r14d, %r8d							
	subl IMAGE_WIDTH_OFFSET(%r12), %r8d 
	subl $1, %r8d
	movl (%r9d, %r8d, 4), %edi		#get top left corner pixel
	/*get r*/
	call get_r
	movl %eax, %ecx
	/*get g*/
	call get_g
	movl %eax, %r8d
	/*get b*/
	call get_b
	movl %eax, %r9d

	/*move r back into edi*/
	movl %rbx, %edi

	call get_largest_abs_diff	#stores largest abs diff return into eax 

	/*checks if diff is greater than 255 or less than 0*/
	addl $128, %eax
	compl $255, %eax
	jg ._if_255 
	compl $0, %eax
	jl ._if_0
._if_255:
	movl $255, %eax
	jmp ._end_if
._if_0:
	movl $0, %eax
	jmp ._end_if
._end_if:
	/*put eax into r11d and move up for rgb*/
	andl 0xFF, %eax		#make sure eax is good range
	movl %eax, %r11d	#scroll up to make room for next color
	shll $8, %r11d
	movl %eax, %r11d
	shll $8, %r11d
	movl %eax, %r11d
	shll $8, %r11d
	jmp .put_data_in	#will add a to r11d and put data into ouput image

.if_gray:
	#check exit loop 

	/*set rgb values to 128, keep a value*/
	movl $128, %r11d	#r value
	shll $8, %r11d		#move up to make room 
	orl $128, %r11d		#g value
	shll $8, %r11d		#move up to make room 
	orl $128, %r11d		#b value
	shll $8, %r11d		#move up to make room

.put_data_in:
	/*extract a value*/
	movq IMAGE_DATA_OFFSET(%r12), %r9
	movq(%r9, %r14, 4), %edi	#insert pixel value at location into edi
	andl 0xFF, %edi								#get a value
	orl %edi, %r11d								#add a value to r11d WHY IS THERE A SQUARE HERE STOP

	/*insert r11d data into correct location in output data*/
	movl IMAGE_DATA_OFFSET(%r13), %r8
	movl %r11d, (%r8,%r14,4)
	
	incq %r14			#increase index

	/*get height times width*/
	movl IMAGE_HEIGHT_OFFSET(%r12), %r9d
	mull IMAGE_WIDTH_OFFSET(%r12), %r9d

	
	compl %r14d, %r9d	# compare index with total height * width
	jl .emboss_loop		# jump if index < height * width

	ret

/*
vim:ft=gas:
*/
