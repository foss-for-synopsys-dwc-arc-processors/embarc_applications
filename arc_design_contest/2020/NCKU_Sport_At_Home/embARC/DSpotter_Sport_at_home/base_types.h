#ifndef	__BASE_TYPES_H
#define	__BASE_TYPES_H

	typedef signed char CHAR;
	typedef unsigned char BYTE;
	typedef signed short SHORT;
	typedef unsigned short WORD;
	typedef unsigned short USHORT;
	typedef signed long LONG;
	typedef unsigned int DWORD;
	typedef signed int INT;
	typedef unsigned int UINT;
	typedef void *HANDLE;
	typedef int BOOL;
	typedef WORD UNICODE;
	typedef void VOID;

#ifndef TRUE
#define TRUE	(1 == 1)
#endif

#ifndef FALSE
#define FALSE	(1 == 0)
#endif

#endif	/* #ifndef	__BASE_TYPES_H */
