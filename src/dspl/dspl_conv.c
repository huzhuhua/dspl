﻿/*
*Copyright (c) 2015, 2016 Sergey Bakhurin
* Digital Signal Processing Library [http://dsplib.org]
*
* This file is part of DSPL.
* 
* DSPL is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
*(at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dspl.h"


void dspl_conv_cmplx_krn(double* aR, double* aI, int na, 
                         double* bR, double* bI, int nb, 
						 double* cR, double* cI);
						 
void dspl_conv_mix_krn(double* aR, double* aI, int na, 
                       double* b, int nb, double* cR, double* cI);
					   
					   
void dspl_conv_real_krn(double* a, int na, double* b, int nb, double* c);



/* Doxygen RU, EN*/
DSPL_API int dspl_conv(double* a, int na, double* b,  int nb, double *c)
{
	double *t;

	size_t bufsize;	
	int f = 0;

	
	if(!a || !b || !c)
		return DSPL_ERROR_PTR;
	if(na < 1 || nb < 1)
		return DSPL_ERROR_SIZE;

	bufsize = (na + nb - 1) * sizeof(double);
	
	if((a != c) && (b != c))
		t = c;
	else
	{
		t = (double*)malloc(bufsize);
	 	f = 1;
	}

	memset(t, 0, bufsize);

	dspl_conv_real_krn(a, na, b, nb, t);

	if(f)
	{
		memcpy(c, t, bufsize);
		free(t);
	}
	return DSPL_OK;	
			
}




/* Doxygen RU, EN*/
DSPL_API int dspl_conv_cmplx(double *aR, double *aI, int na, double *bR, double *bI, int nb, 
					double *cR, double *cI)
{
	double *tR = NULL;
	double *tI = NULL;
	size_t bufsize;	
	int fR = 0;
	int fI = 0;
	
	if(!aR || !bR || !cR || ((aI || bI) && !cI))
		return DSPL_ERROR_PTR;
	if(na < 1 || nb < 1)
		return DSPL_ERROR_SIZE;

	bufsize = (na + nb - 1) * sizeof(double);
	
	if((aR != cR) && (bR != cR))
		tR = cR;
	else
	{
		tR = (double*)malloc(bufsize);
	 	fR = 1;
	}

	if((aI != cI) && (bI != cI))
		tI = cI;
	else
	{
		if(cI)
		{
			tI = (double*)malloc(bufsize);
	 		fI = 1;
		}
	}
	memset(tR, 0, bufsize);
	if(tI)
		memset(tI, 0, bufsize);

	if(aI && bI)
		dspl_conv_cmplx_krn(aR, aI, na, bR, bI, nb, tR, tI);
	if(aI && !bI)
		dspl_conv_mix_krn(aR, aI, na, bR, nb, tR, tI);
	if(!aI && bI)
		dspl_conv_mix_krn(bR, bI, nb, aR, na, tR, tI);
	if(!aI && !bI)	
		dspl_conv_real_krn(aR, na, bR, nb, tR);

	if(fR)
	{
		memcpy(cR, tR, bufsize);
		free(tR);
	}
	if(fI)
	{
		memcpy(cI, tI, bufsize);
		free(tI);
	}
	return DSPL_OK;	
			
}



/* Linear convolution for complex vctors */
void dspl_conv_cmplx_krn(double* aR, double* aI, int na, 
                         double* bR, double* bI, int nb, 
						 double* cR, double* cI)
{
	int k;
	int n;
	for(k = 0; k < na; k++)
	{
		for(n = 0; n < nb; n++)
		{
			cR[k+n] += aR[k]*bR[n] - aI[k]*bI[n];
			cI[k+n] += aR[k]*bI[n] + aI[k]*bR[n];
		}
	}
}




/* Linear convolution if one vector is complex bu other is real */
void dspl_conv_mix_krn(double* aR, double* aI, int na, 
                       double* b, int nb, double* cR, double* cI)
{
	int k;
	int n;
	for(k = 0; k < na; k++)
	{
		for(n = 0; n < nb; n++)
		{
			cR[k+n] += aR[k]*b[n];
			cI[k+n] += aI[k]*b[n];
		}
	}
}




/* Linear convolution if both vectors a and b are real */
void dspl_conv_real_krn(double* a, int na, double* b, int nb, double* c)
{
	int k;
	int n;
	for(k = 0; k < na; k++)
		for(n = 0; n < nb; n++)
			c[k+n] += a[k]*b[n];
}


