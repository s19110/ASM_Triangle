.data
	Point struct
		x real8 ?
		y real8 ?
	Point ends

	Triangle struct
		a Point { }
		b Point { }
		c Point { }
	Triangle ends

.code
	getLengthASM proc
		SUBSD xmm0, xmm2	;(x1-x2)
		SUBSD xmm1, xmm3	;(y1-y2)
		MULSD xmm0, xmm0	;(x1-x2)^2
		MULSD xmm1, xmm1	;(y1-y2)^2
		ADDSD xmm0, xmm1	;(x1-x2)^2+(y1-y2)^2
		SQRTSD xmm0, xmm0	;sqrt((x1-x2)^2+(y1-y2)^2)
		RET
	getLengthASM endp

	getStructLengthASM proc
		MOVSD xmm0, [rcx].Point.x
		MOVSD xmm1, [rcx].Point.y
		SUBSD xmm0, [rdx].Point.x
		SUBSD xmm1, [rdx].Point.y
		MULSD xmm0, xmm0
		MULSD xmm1, xmm1
		ADDSD xmm0, xmm1
		SQRTSD xmm0, xmm0
		RET
	getStructLengthASM endp
	
	isTriangleASM proc
		;Clear registries
		XOR rax, rax
		XOR rcx, rcx ;Skipping rbx because it's used by the C++ compiler

		VADDSD xmm3, xmm0, xmm1		;xmm3 = a+b
		VADDSD xmm4, xmm0, xmm2		;xmm4 = a+c
		VADDSD xmm5, xmm1, xmm2		;xmm5 = b+c
		COMISD xmm3, xmm2			;a+b ? c
		SETG al						;al == 1 if a+b>c
		COMISD xmm4, xmm1			;a+c ? b
		SETG cl						;cl == 1 if a+c>b
		AND al, cl					;a+b>c && a+c>b
		COMISD xmm5, xmm2			;b+c ? a
		SETG cl						;cl == 1 if b+c>a
		AND al, cl					;a+b>c && a+c>b && b+c>a

		ret
	isTriangleASM endp

	isTriangleStructASM proc
		push rbx	 ;We want to preserve the value of rbx and restore it before exiting the procedure to ensure C++ compiler runs properly
		MOV rax, rcx ;Moving triangle pointer to rax - rax = Point.a
		MOV rdx, rcx ;Pointer to triangle = Pointer to Point.a
		ADD rcx, 16	 ;rcx = Point.b
		ADD rdx, 32  ;rdx = Point.c

		CALL getStructLengthASM ;xmm0 = getStructLength(b,c)
		MOVSD xmm3, xmm0		;xmm0 = xmm3 (b,c) -> xmm1 is used by getStructLengthASM, we want to preserve the return value
		MOV rbx, rcx			;rbx = Point.b
		MOV rcx, rax			;rcx = Point.a
		
		CALL getStructLengthASM	;xmm0 = getStructLength(a,c)
		MOVSD xmm2, xmm0		;xmm2 = xmm0 (a,c)
		MOV rdx, rbx			;rdx = Point.b
		CALL getStructLengthASM ;xmm0 = getStructLength(a,b)
		MOVSD xmm1, xmm3		;xmm1 = xmm3 - when calling isTriangleASM, we want to store lengths in xmm0, xmm1, xmm2
		CALL isTriangleASM		;xmm0=(a,b), xmm1=(b,c), xmm2=(a,c)
		pop rbx
		ret
	isTriangleStructASM endp
end