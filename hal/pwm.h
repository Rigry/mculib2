/**     Работа с ШИМ
 *      При объявление требует параметры шаблонов
 *          статический класс аппаратного таймера
 *          статический класс пина
 *      Конструктор инициализирует пин и таймер
 *      Пары таймер-пин конкретезируются путём инициализации константы channel для пары
 *      При неверном задании пары таймер-пин не компилируеться
 */

#pragma once

#include "TIM.h"

#include "pin.h"
#include "literals.h"


template <class TIM_, class Pin_>
class PWM
{
public:
    PWM () : d(0)
    {
        static_assert (
            channel != 0,
            "Вывод контроллера не поддерживает функцию ШИМ с этим таймером"
        );
        init();
    }
    inline void outEnable ()  { TIM_::template CompareEnable  <channel>(); }
    inline void outDisable () { TIM_::template CompareDisable <channel>(); }
    inline void outToggle ()  { TIM_::template CompareToggle  <channel>(); }
    inline bool isOutEnable ()    { return TIM_::template IsCompareEnable<channel>(); }
    inline void setFreq (uint32_t f)
    {
        extern const uint32_t fCPU;
        if ( (f != this->freq) && (f != 0) ) {
            this->freq = f;
            countTo = fCPU / f - 1;
            TIM_::SetAutoReloadValue (countTo);
            setD (d);
        }
    }
    
    // d - в процентах
    inline void setD (uint8_t d)
    {
        this->d = d;
        TIM_::template SetCompareValue <channel> (countTo * d / 100);
    }



private:
    // частота в Гц
    uint16_t freq;
    // коэффициент заполнения в процентах
    uint16_t d;
    // таймер считает до, зависит от частоты
    uint32_t countTo;

    // номер канала таймера
    static const uint8_t channel = Channel<TIM_,Pin_>();//PWM_HAL::Channel<TIM_,Pin>().val;
    // static const AFR_t::AF altFun = AltFunc<Timer,Pin>();
    void init (void)
    {
        Pin_::Port::ClockEnable();
        TIM_::clockEnable();


       
        Pin_::Configure ( Pin_::Mode::Alternate,
                         Pin_::OutType::PushPull,
                         Pin_::OutSpeed::High,
                         Pin_::PullResistor::No);
        Pin_::template SetAltFunc <AltFunc<TIM_,Pin_>()> ();

        TIM_::template SetCompareMode <TIM_::CompareMode::PWMmode, channel> ();
        TIM_::template PreloadEnable <channel> ();
        TIM_::AutoReloadEnable();
        TIM_::MainOutputEnable();
        TIM_::CounterEnable();
    }

};





