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

gContainer.RegisterFactory<ProcessorType, Processor64>();
gContainer.RegisterFactory<ProcessorType, Processor86>();

gContainer.RegisterFactory<ProcessorBranding, I8Branding>();
gContainer.RegisterFactory<ProcessorFamily, RazorFamily>();

gContainer.RegisterInstance<IRam::DoubleDateRate>(std::make_shared<IRam::DoubleDateRate>(IRam::DDR4));


gContainer.RegisterFactory<IProcessor, IntelProcessor, ProcessorType, ProcessorBranding>();

gContainer.RegisterFactory<IProcessor, AmdProcessor, ProcessorType, ProcessorFamily>();

gContainer.RegisterFactory<IRam, KingstonRam, IRam::DoubleDateRate>();

gContainer.RegisterFactory<Computer, Computer, IProcessor, IRam>();

gContainer.GetObject<Computer>()->Specs();

    return 0;
}
