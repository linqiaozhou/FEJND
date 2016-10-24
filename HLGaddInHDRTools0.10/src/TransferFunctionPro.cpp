/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * <OWNER> = Apple Inc.
 * <ORGANIZATION> = Apple Inc.
 * <YEAR> = 2014
 *
 * Copyright (c) 2014, Apple Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the <ORGANIZATION> nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 *************************************************************************************
 * \file TransferFunctionPQ.cpp
 *
 * \brief
 *    TransferFunctionPQ Class
 *
 * \author
 *     - Alexis Michael Tourapis         <atourapis@apple.com>
 *
 *************************************************************************************
 */

//-----------------------------------------------------------------------------
// Include headers
//-----------------------------------------------------------------------------

#include "Global.H"
#include "TransferFunctionPro.H"
#include<iostream>
#include <time.h>
#include<fstream>
using namespace std;
#define Method 1
//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
//void writeGamma(float *gamma, int num, char *gammaInfile="gammafile.txt");
//void writeHistgram(Frame *out);
//-----------------------------------------------------------------------------
// Constructor/destructor
//-----------------------------------------------------------------------------

TransferFunctionPro::TransferFunctionPro() {
	m_Time = 0;
	//M=0.5时对应的数据
	/*m_a = 0.3844;
	m_b = 1.6461;
	m_c = 0.6278;*/
	//Max=0.9是的数据
	/*m_a = 0.2255;
	m_b = 2.2182;
	m_c = 0.6845;*/
	//Max=0.8的数据
	/*m_a = 1829883 / 15680000;
	m_b = 9545 / 3479;
	m_c = 121171 / 175616;*/
	//Max=0.85对应的数据
	/*m_a = 1464516361 / 8847360000;
	m_b = 59315 / 23856;
	m_c = 9808231 / 14155776;*/
	//不连续对应的数据
	m_a = 0.9828;
	m_b = -1.0998;
	m_c = 0;


	Hival = 10000;
	Loval = 1;
	N = 1024;
	M = 0.5;  //原来是等于1
	P = (M / N)*(Hival / Loval);
	m_Lmax = 12;
	
	//m_new_a = 0.25215621;
	//m_new_b = -0.00862483;
	//m_new_c = 0.49783452;
	//m_f = 0.0083;
	//m_g = 0.9;
	m_new_a = 0.13911898;
	m_new_b = 0.60251721;
	m_new_c = 0.82835167;
	m_f = 1 / 180;
	m_g = 14 / 15;
}


TransferFunctionPro::~TransferFunctionPro() {
}

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------


void TransferFunctionPro::forward(Frame* out, const Frame *inp, int component) {
	if (inp->m_isFloat == TRUE && out->m_isFloat == TRUE && inp->m_compSize[component] == out->m_compSize[component]) {
		for (int index = 0; index < inp->m_compSize[component]; index++) {
			//      out->m_floatComp[component][index] = (float) forward((double) inp->m_floatComp[component][index]);  直接注释掉了，因为这里不会调用
		}
	}
	else if (inp->m_isFloat == FALSE && out->m_isFloat == FALSE && inp->m_size == out->m_size && inp->m_bitDepth == out->m_bitDepth) {
		out->copy((Frame *)inp, component);
	}
}

void TransferFunctionPro::forward(Frame* out, const Frame *inp) {
	cout << "Proposed  Inverse" << endl;
	if (inp->m_isFloat == TRUE && out->m_isFloat == TRUE && inp->m_size == out->m_size)  {
		double V;
		for (int index = 0; index < inp->m_size; index++) {
			// out->m_floatData[index] = (float) forward((double) inp->m_floatData[index]);
			V = (double)inp->m_floatData[index];
			//out->m_floatData[index] = V < 0.5 ? (V* V * 4.0) : ((V-m_c)*m_Lmax/(m_a*P+(V-m_c)*(1-P)));
			if (Method==1)
				out->m_floatData[index] = V < 0.5 ? (V* V * 4.0) : ((V-m_c)*(P*m_b-m_b-m_Lmax)-m_a*P*m_b)/((V-m_c)*(P-1)-m_a*P);
			else
			{
				//cout << "Method 2" << endl;
				if (V <= 0.7)
					out->m_floatData[index] = V* V * 4.0;
				else if (V>0.7&&V < 0.95)
					out->m_floatData[index] = exp((V - m_new_c) / m_new_a) + m_new_b;
				else
					out->m_floatData[index] = (V - m_g) / m_f;

			}
			out->m_floatData[index] = out->m_floatData[index] / 12;
		}
	}
	else if (inp->m_isFloat == FALSE && out->m_isFloat == FALSE && inp->m_size == out->m_size && inp->m_bitDepth == out->m_bitDepth) {
		out->copy((Frame *)inp);
	}
}

void TransferFunctionPro::inverse(Frame* out, const Frame *inp, int component) {
  // In this scenario, we should likely copy the frame number externally
	if (inp->m_isFloat == TRUE && out->m_isFloat == TRUE && inp->m_compSize[component] == out->m_compSize[component]) {
		for (int index = 0; index < inp->m_compSize[component]; index++) {
			// out->m_floatComp[component][index] = (float) inverse((double) inp->m_floatComp[component][index]);    直接注释掉了，因为这里不会调用
		}
	}
	else if (inp->m_isFloat == FALSE && out->m_isFloat == FALSE && inp->m_size == out->m_size && inp->m_bitDepth == out->m_bitDepth) {
		out->copy((Frame *)inp, component);
	}
}

void TransferFunctionPro::inverse(Frame* out, const Frame *inp) {
	cout << "Proposed  Forward" << endl;
	//cout << inp->m_size << endl;
	if (inp->m_isFloat == TRUE && out->m_isFloat == TRUE && inp->m_size == out->m_size)  {
		double L;
		double start, end, cost;
		start = clock();
		for (int index = 0; index < inp->m_size; index++) {
			L = 12 * (double)inp->m_floatData[index];
			out->m_floatData[index] = L < 1.0 ? 0.5 * sqrt(L) : m_a*P*(L - m_b) / (P*(L - m_b) - (L - m_b) + m_Lmax) + m_c;
			//out->m_floatData[index] = (float) inverse((double) inp->m_floatData[index]);
			//out->m_floatData[index]=L < 1.0 ? 0.5 * sqrt(L) : m_a*P*L/(P*L-L+m_Lmax) + m_c; 
			//if (Method == 1)
			//	out->m_floatData[index] = L < 1.0 ? 0.5 * sqrt(L) : m_a*P*(L - m_b) / (P*(L - m_b) - (L - m_b) + m_Lmax) + m_c;
			//else
			//{
			//	//cout << "Method 2" << endl;
			//	if (L<=1)
			//	{
			//		out->m_floatData[index] = 0.7 * sqrt(L);
			//	}
			//	else if (L>1 && L <= 3)
			//	{
			//		out->m_floatData[index] = m_new_a * log(L - m_new_b) + m_new_c;
			//	}
			//	else
			//	{
			//		out->m_floatData[index] = m_f*L + m_g;
			//	}

			//}
			
		}
		end = clock();
		cost = end - start;
		cout << "************" << cost << "****************" << endl;
		m_Time += cost;
		cout << "************" << m_Time << "****************" << endl;
		ofstream f("time.txt",ios::in|ios::out);
		f << m_Time<<endl;
		f.close();
		

	}
	else if (inp->m_isFloat == FALSE && out->m_isFloat == FALSE && inp->m_size == out->m_size && inp->m_bitDepth == out->m_bitDepth) {
		out->copy((Frame *)inp);
	}
}



/*void writeHistgram(Frame *out)
{
	int *gHist = new int[1024];
	int imageSize = out->m_compSize[0];
	int temp=0;
	int max;
	float ingamma[1024]={0.0};
	//	float gamma[1024]={0.0};
	for (int i = 0; i < 1024; i++)
	{

		gHist[i]=0;    // histogram
	}

	for (int i = 0; i < imageSize; i++)
	{
		temp=(int)(1024*(out->m_floatComp[0][i]+out->m_floatComp[1][i]+out->m_floatComp[2][i])/3);
		gHist[temp]++;    // histogram
	}

	for (int i = 0; i < 1024; i++)
	{
		if(gHist[1023-i]>400)
		{max=1023-i;break;}   
	}

	for (int i = 0; i < 1024; i++)
	{
		ingamma[i]=gHist[i];
	}
	writeGamma(ingamma,1024,"framehistgram.txt");
}*/
//-----------------------------------------------------------------------------
// End of file
//-----------------------------------------------------------------------------
