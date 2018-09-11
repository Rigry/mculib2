#define STM32F030x6

#include "RCC.h"
#include "adc_periph.h"
#include <iostream>
#include <type_traits>
#include <thread>

struct MockPointer
{
   static ADC1::CMSIS_type* p;
   static auto get() { return reinterpret_cast<ADC1::periph_type*>(p); }
};
ADC1::CMSIS_type mock;
ADC1::CMSIS_type* MockPointer::p = &mock;
using TADC = template_ADC<0, MockPointer>;

// тесты возвращают true, если прошли
bool enable()
{
   mock.CR = 0;
   std::thread{TADC::enable}.detach();
   while ((mock.CR & ADC_CR_ADEN_Msk) == 0)
   {
   }
   return true;
}

bool disable()
{
   mock.CR = 0;
   std::thread{TADC::disable}.detach();
   while ((mock.CR & ADC_CR_ADSTP_Msk) == 0)
   {
   }
   mock.CR &= ~ADC_CR_ADSTP_Msk;
   while ((mock.CR & ADC_CR_ADDIS_Msk) == 0)
   {
   }
   return true;
}

bool is_disable_1()
{
   mock.CR = 0;
   return not TADC::is_disable();
}

bool is_disable_2()
{
   mock.CR |= ADC_CR_ADDIS_Msk;
   return TADC::is_disable();
}

bool start()
{
   mock.CR = 0;
   TADC::start();
   return mock.CR & ADC_CR_ADSTART_Msk;
}

bool DMAenable()
{
   mock.CFGR1 = 0;
   TADC::DMAenable();
   return mock.CFGR1 & ADC_CFGR1_DMAEN_Msk;
}

bool setCircularDMA()
{
   mock.CFGR1 = 0;
   TADC::setCircularDMA();
   return mock.CFGR1 & ADC_CFGR1_DMACFG_Msk;
}

bool setContinuousMode()
{
   mock.CFGR1 = 0;
   TADC::setContinuousMode();
   return mock.CFGR1 & ADC_CFGR1_CONT_Msk;
}


bool setClock()
{
   mock.CFGR2 = 0;
   bool good {true};

   TADC::set (TADC::Clock::PCLKdiv4);
   good &= not (mock.CFGR2 & ADC_CFGR2_CKMODE_0)
       and     (mock.CFGR2 & ADC_CFGR2_CKMODE_1);

   TADC::set (TADC::Clock::PCLKdiv2);
   good &=     (mock.CFGR2 & ADC_CFGR2_CKMODE_0)
       and not (mock.CFGR2 & ADC_CFGR2_CKMODE_1);

   TADC::set (TADC::Clock::Dedicated14MHzClock);
   good &= not (mock.CFGR2 & ADC_CFGR2_CKMODE_0)
       and not (mock.CFGR2 & ADC_CFGR2_CKMODE_1);
   return good;
}

bool setResolution()
{
   mock.CFGR1 = 0;
   bool good {true};

   TADC::set (TADC::Resolution::_6bits);
   good &=     (mock.CFGR1 & ADC_CFGR1_RES_0)
       and     (mock.CFGR1 & ADC_CFGR1_RES_1);

   TADC::set (TADC::Resolution::_8bits);
   good &= not (mock.CFGR1 & ADC_CFGR1_RES_0)
       and     (mock.CFGR1 & ADC_CFGR1_RES_1);

   TADC::set (TADC::Resolution::_10bits);
   good &=     (mock.CFGR1 & ADC_CFGR1_RES_0)
       and not (mock.CFGR1 & ADC_CFGR1_RES_1);

   TADC::set (TADC::Resolution::_12bits);
   good &= not (mock.CFGR1 & ADC_CFGR1_RES_0)
       and not (mock.CFGR1 & ADC_CFGR1_RES_1);

   return good;
}

bool setSampleTime()
{
   mock.SMPR = 0;
   bool good {true};

   TADC::set (TADC::SampleTime::_239_5CLOCK);
   good &=     (mock.SMPR & ADC_SMPR_SMP_0)
       and     (mock.SMPR & ADC_SMPR_SMP_1)
       and     (mock.SMPR & ADC_SMPR_SMP_2);

   TADC::set (TADC::SampleTime::_71_5CLOCK);
   good &= not (mock.SMPR & ADC_SMPR_SMP_0)
       and     (mock.SMPR & ADC_SMPR_SMP_1)
       and     (mock.SMPR & ADC_SMPR_SMP_2);

   TADC::set (TADC::SampleTime::_55_5CLOCK);
   good &=     (mock.SMPR & ADC_SMPR_SMP_0)
       and not (mock.SMPR & ADC_SMPR_SMP_1)
       and     (mock.SMPR & ADC_SMPR_SMP_2);

   TADC::set (TADC::SampleTime::_41_5CLOCK);
   good &= not (mock.SMPR & ADC_SMPR_SMP_0)
       and not (mock.SMPR & ADC_SMPR_SMP_1)
       and     (mock.SMPR & ADC_SMPR_SMP_2);

   TADC::set (TADC::SampleTime::_28_5CLOCK);
   good &=     (mock.SMPR & ADC_SMPR_SMP_0)
       and     (mock.SMPR & ADC_SMPR_SMP_1)
       and not (mock.SMPR & ADC_SMPR_SMP_2);

   TADC::set (TADC::SampleTime::_13_5CLOCK);
   good &= not (mock.SMPR & ADC_SMPR_SMP_0)
       and     (mock.SMPR & ADC_SMPR_SMP_1)
       and not (mock.SMPR & ADC_SMPR_SMP_2);
 
   TADC::set (TADC::SampleTime::_7_5CLOCK);
   good &=     (mock.SMPR & ADC_SMPR_SMP_0)
       and not (mock.SMPR & ADC_SMPR_SMP_1)
       and not (mock.SMPR & ADC_SMPR_SMP_2);

   TADC::set (TADC::SampleTime::_1_5CLOCK);
   good &= not (mock.SMPR & ADC_SMPR_SMP_0)
       and not (mock.SMPR & ADC_SMPR_SMP_1)
       and not (mock.SMPR & ADC_SMPR_SMP_2);

   static_assert ( TADC::SampleTime::Default == TADC::SampleTime::_239_5CLOCK );

   return good;
}

bool is_ready()
{
   bool good {true};
   mock.ISR = 0;
   good &= not TADC::is_ready();

   mock.ISR |= ADC_ISR_ADRDY_Msk;
   good &= TADC::is_ready();

   return good;
}


bool setBusy()
{
   mock.ISR = 0;
   TADC::setBusy();
   return mock.ISR & ADC_ISR_ADRDY_Msk;
}

bool stop()
{
   mock.CR = 0;
   TADC::stop();
   return mock.CR & ADC_CR_ADSTP_Msk;
}

bool is_stoping()
{
   bool good {true};
   mock.CR = 0;
   good &= not TADC::is_stoping();

   mock.CR |= ADC_CR_ADSTP_Msk;
   good &= TADC::is_stoping();

   return good;
}

bool getDataAdr()
{
   return TADC::getDataAdr() == reinterpret_cast<size_t>(&mock.DR);
}

bool setChannel()
{
   bool good {true};
   mock.CHSELR = 0;
   TADC::setChannel<0>();
   good &=     (mock.CHSELR & ADC_CHSELR_CHSEL0_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL1_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL2_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL3_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL4_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL5_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL6_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL7_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL8_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL9_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL10_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL11_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL12_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL13_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL14_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL15_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL16_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL17_Msk);

   TADC::setChannel<5>();
   good &=     (mock.CHSELR & ADC_CHSELR_CHSEL0_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL1_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL2_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL3_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL4_Msk)
       and     (mock.CHSELR & ADC_CHSELR_CHSEL5_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL6_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL7_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL8_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL9_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL10_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL11_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL12_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL13_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL14_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL15_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL16_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL17_Msk);

   TADC::setChannel<17>();
   good &=     (mock.CHSELR & ADC_CHSELR_CHSEL0_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL1_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL2_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL3_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL4_Msk)
       and     (mock.CHSELR & ADC_CHSELR_CHSEL5_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL6_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL7_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL8_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL9_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL10_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL11_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL12_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL13_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL14_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL15_Msk)
       and not (mock.CHSELR & ADC_CHSELR_CHSEL16_Msk)
       and     (mock.CHSELR & ADC_CHSELR_CHSEL17_Msk);

   // TADC::setChannel<18>(); // ошибка компиляции

   return good;

}

static_assert ( std::is_same_v <ADC1::DefaultStream, DMA1channel1> );

static_assert ( ADC1::channel<PA0>() == 0  );
static_assert ( ADC1::channel<PA1>() == 1  );
static_assert ( ADC1::channel<PA2>() == 2  );
static_assert ( ADC1::channel<PA3>() == 3  );
static_assert ( ADC1::channel<PA4>() == 4  );
static_assert ( ADC1::channel<PA5>() == 5  );
static_assert ( ADC1::channel<PA6>() == 6  );
static_assert ( ADC1::channel<PA7>() == 7  );
static_assert ( ADC1::channel<PB0>() == 8  );
static_assert ( ADC1::channel<PB1>() == 9  );
static_assert ( ADC1::channel<PC0>() == 10 );
static_assert ( ADC1::channel<PC1>() == 11 );
static_assert ( ADC1::channel<PC2>() == 12 );
static_assert ( ADC1::channel<PC3>() == 13 );
static_assert ( ADC1::channel<PC4>() == 14 );
static_assert ( ADC1::channel<PC5>() == 15 );

static_assert ( ADC1::channel<PC6>() == 255 );

static_assert ( ADC1::PINenabled<PC5>() );
static_assert ( not ADC1::PINenabled<PC6>() );


static_assert ( ADC1::DMAenabled<DMA1channel1>() );
static_assert ( not ADC1::DMAenabled<DMA1channel2>() );



int main()
{
   std::cout << '\n'
             << "Тесты класса ADC для STM32F0:" << std::endl;

   // \033[32mТЕКСТ\033[0m - для цвета
   std::cout << "ADC::enable                  " << (enable()                  ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::disable                 " << (disable()                 ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::is_disable_1            " << (is_disable_1()            ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::is_disable_2            " << (is_disable_2()            ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::start                   " << (start()                   ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::DMAenable               " << (DMAenable()               ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::setCircularDMA          " << (setCircularDMA()          ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::setContinuousMode       " << (setContinuousMode()       ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::setClock                " << (setClock()                ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::setResolution           " << (setResolution()           ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::setSampleTime           " << (setSampleTime()           ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::is_ready                " << (is_ready()                ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::setBusy                 " << (setBusy()                 ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::stop                    " << (stop()                    ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::is_stoping              " << (is_stoping()              ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::getDataAdr              " << (getDataAdr()              ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << "ADC::setChannel              " << (setChannel()              ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << std::endl;
}
