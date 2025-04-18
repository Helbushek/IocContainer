#include "IOC_Contaner.h"
#include <cstring>
#include <Computer.h>

int main(int argc, const char *argv[])
{
//    //------Example #1----------------
//    //Injector injector;
//    IOCContainer injector;

//           // Регистрируем IHello с классом Hello, т.о. каждый раз запрашивая IHello получаем объект Hello.
//    injector.RegisterInstance<IHello, Hello>();
//    auto helloInstance = injector.GetObject<IHello>();
//    helloInstance->hello();
//    injector.RegisterInstance<IHello, Privet>();

//           //Здесь, после регистрации получим объект Privet
//    helloInstance = injector.GetObject<IHello>();
//    helloInstance->hello();

//           //------Example #2----------------

//    gContainer.RegisterInstance<IAmAThing, TheThing>();
//    gContainer.RegisterFactory<IAmTheOtherThing, TheOtherThing, IAmAThing>();

//    gContainer.GetObject<IAmAThing>()->TestThis();
//    gContainer.GetObject<IAmTheOtherThing>()->TheOtherTest();

//           //Опять запршиваем объект,после последней регистрации получим объект Privet
//    helloInstance = injector.GetObject<IHello>();
//    helloInstance->hello();

//    gContainer.RegisterInstance<IProcessor, IntelProcessor>();
    gContainer.RegisterFactory<IProcessor, IntelProcessor, Processor64, I8Branding>();

    gContainer.GetObject<IProcessor>()->getComponentInfo();

    return 0;
}
