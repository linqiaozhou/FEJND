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
 * \file TransferFunction.cpp
 *
 * \brief
 *    Base Class for transfer function application
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
#include "TransferFunction.H"
#include "TransferFunctionNull.H"
#include "TransferFunctionPQ.H"
#include "TransferFunctionAPQ.H"
#include "TransferFunctionPH.H"
#include "TransferFunctionHG.H"
#include "TransferFunctionNormalize.H"
#include "TransferFunctionPower.H"
#include "TransferFunctionLogRTM.H"
#include "TransferFunctionPro.H"
#include "TransferFunctionHLG.H"
#include "TransferFunctionMy_HG.H"

#ifdef __SIM2_SUPPORT_ENABLED__
//#include "TransferFunctionSim2.H"
#endif

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------
TransferFunction *TransferFunction::create(int method, int singleStep, float scale, float systemGamma, float minValue, float maxValue) {
  TransferFunction *result = NULL;
  
  switch (method){
    case TF_NULL:
      result = new TransferFunctionNull();
      break;
    case TF_APQ:
	 if (singleStep)
		result = new TransferFunctionAPQ(scale, minValue, maxValue);
    
      else
        result = new TransferFunctionAPQ(minValue, maxValue);
      break;
    case TF_PQ:
		if (singleStep)
			result = new TransferFunctionPQ(scale);
      else
        result = new TransferFunctionPQ();
      break;
    /*case TF_PH:
      result = new TransferFunctionPH(); //改成自己的函数
	  */
	case TF_PH:
	// result = new TransferFunctionPro();   //自己写的HLG
	 result = new TransferFunctionHLG();
      break;
    case TF_HG:
      result = new TransferFunctionHG(systemGamma);
	 // result = new TransferFunctionMy_HG(systemGamma);
      break;
	
    case TF_NORMAL:
      result = new TransferFunctionNormalize(scale);
      break;
    case TF_POWER:
      result = new TransferFunctionPower(systemGamma, scale);
      break;
#ifdef __SIM2_SUPPORT_ENABLED__
    case TF_SIM2:
    //  result = new TransferFunctionSim2();
      break;
#endif
    default:
      fprintf(stderr, "\nUnsupported Transfer Function %d\n", method);
      exit(EXIT_FAILURE);
  }
  
  
  return result;
}

//-----------------------------------------------------------------------------
// End of file
//-----------------------------------------------------------------------------
