#pragma once

#include "ADC.h"

///////////////////////////////////////////////////////////////////////////////
//  00   00       0000000
//  00   00       00   00
//  00   00       00   00
//  0000000       00   00
//       00       00   00
//       00       0000000
///////////////////////////////////////////////////////////////////////////////
#if defined(STM32F405xx) || defined(STM32F030x6)

template <uint32_t adr>
void ADCx<adr>::clockEnable()
{
   RCC::template clockEnable<ADCx<adr>>();
}


template <uint32_t adr>
uint32_t ADCx<adr>::getDataAdr()
{
   return adr + data().Offset;
}


#endif


///////////////////////////////////////////////////////////////////////////////
//  00   00
//  00   00
//  00   00
//  0000000
//       00
//       00
///////////////////////////////////////////////////////////////////////////////

#if defined(STM32F405xx)

template <uint32_t adr>
void ADCx<adr>::setClock ( Clock val )
{
   ADCC::setClock (val);
}

template <uint32_t adr>
void ADCx<adr>::enable()
{
   SET(control2(), ADON);
}


template <uint32_t adr>
void ADCx<adr>::disable()
{
   CLEAR(control2(), ADON);
}


template <uint32_t adr>
bool ADCx<adr>::isDisable()
{
   return IS_CLEAR(control2(), ADON);
}


template <uint32_t adr>
void ADCx<adr>::start()
{
   SET(control2(), SWSTART);
}


template <uint32_t adr>
void ADCx<adr>::DMAenable()
{
   SET(control2(), DMA);
}


template <uint32_t adr>
void ADCx<adr>::setCircularDMA()
{
   SET(control2(), DDS);
}


template <uint32_t adr>
void ADCx<adr>::setContinuousMode()
{
   SET(control2(), CONT);
}


template <uint32_t adr>
void ADCx<adr>::setResolution ( Resolution val )
{
   uint32_t tmp = control().reg;
   tmp &= ~_2BIT_TO_MASK(control(), RES);
   tmp |= VAL_TO_MASK(control(), RES, val);
   control().reg = tmp;
}


template <uint32_t adr>
template <uint8_t channel>
void ADCx<adr>::setSampleTime ( SampleTime val )
{
   constexpr std::size_t n = channel > 9 ? 0 : 1;
   uint32_t tmp = sampleTime().reg[n];
   constexpr std::size_t pos = channel > 9 ? (channel - 10) * 3 : channel * 3;
   tmp &= ~(0b111 << pos);
   tmp |= val << pos;
   sampleTime().reg[n] = tmp;
}


template <uint32_t adr>
constexpr typename ADCx<adr>::Channels
ADCx<adr>::DMAchannel()
{
   static_assert (
      adr == ADC1_BASE or adr == ADC2_BASE or adr == ADC3_BASE,
      "Такого базового адреса нет ни для одного из ADC"
   );
   return adr == ADC1_BASE ? Channels::_0 :
          adr == ADC2_BASE ? Channels::_1 :
                             Channels::_2 ;
}




///////////////////////////////////////////////////////////////////////////////
//  0000000
//  00   00
//  00   00
//  00   00
//  00   00
//  0000000
///////////////////////////////////////////////////////////////////////////////
#elif defined(STM32F030x6)

template <uint32_t adr>
void ADCx<adr>::disable()
{
   stop();
   while ( isStoping() ) { }
   SET(control(), ADDIS);
   while ( not isDisable() ) { }
}


template <uint32_t adr>
bool ADCx<adr>::isDisable()
{
   return IS_SET(control(), ADDIS);
}


template <uint32_t adr>
bool ADCx<adr>::isReady()
{
   return IS_SET(status(), ADRDY);
}


template <uint32_t adr>
void ADCx<adr>::setBusy()
{
   SET(status(), ADRDY);
}


template <uint32_t adr>
void ADCx<adr>::stop()
{
   SET(control(), ADSTP);
}


template <uint32_t adr>
void ADCx<adr>::start()
{
   SET(control(), ADSTART);
}


template <uint32_t adr>
bool ADCx<adr>::isStoping()
{
   return IS_SET(control(), ADSTP);
}

template <uint32_t adr>
void ADCx<adr>::setClock ( Clock val )
{
   uint32_t tmp = conf2().reg;
   tmp &= ~_2BIT_TO_MASK(conf2(), CKMODE);
   tmp |= VAL_TO_MASK(conf2(), CKMODE, val);
   conf2().reg = tmp;
}


template <uint32_t adr>
void ADCx<adr>::enable()
{
   if (isReady())
      setBusy();
   SET(control(), ADEN);
   while ( not isReady() ) { }
}


template <uint32_t adr>
void ADCx<adr>::DMAenable()
{
   SET(conf1(), DMAEN);
}


template <uint32_t adr>
void ADCx<adr>::setCircularDMA()
{
   SET(conf1(), DMACFG);
}


template <uint32_t adr>
void ADCx<adr>::setContinuousMode()
{
   SET(conf1(), CONT);
}


template <uint32_t adr>
void ADCx<adr>::setResolution ( Resolution val )
{
   uint32_t tmp = conf1().reg;
   tmp &= ~_2BIT_TO_MASK(conf1(), RES);
   tmp |= VAL_TO_MASK(conf1(), RES, val);
   conf1().reg = tmp;
}


template <uint32_t adr>
void ADCx<adr>::setSampleTime ( SampleTime val )
{
   uint32_t tmp = sampleTime().reg;
   tmp &= ~_3BIT_TO_MASK(sampleTime(), SMP);
   tmp |= VAL_TO_MASK(sampleTime(), SMP, val);
   sampleTime().reg = tmp;
}


template <uint32_t adr>
void ADCx<adr>::setChannel (uint8_t val )
{
   channelSelect().reg |= ((uint32_t)1 << val);
}

#endif