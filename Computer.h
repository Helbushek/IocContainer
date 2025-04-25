#pragma once
#include <memory>
#include <iostream>
#include <string>

class PcComponent
{
  public:
    virtual ~PcComponent(){};
    virtual void getComponentInfo() const = 0;
};

class ProcessorType
{
  public:
    ProcessorType(){};
    ~ProcessorType(){};
    virtual std::string getType() const = 0;
};

class Processor64 : public ProcessorType
{
    std::string getType() const override
    {
        return "x64";
    }
};

class Processor86 : public ProcessorType
{
    std::string getType() const override
    {
        return "x86";
    }
};

class IProcessor : public PcComponent
{
  public:
    IProcessor(){};
    ~IProcessor(){};

    IProcessor(std::shared_ptr<ProcessorType> processorType) : processorType(processorType)
    {
    }

    std::string getProcessorType() const
    {
        return processorType->getType();
    }

  protected:
    std::shared_ptr<ProcessorType> processorType;
};

class ProcessorFamily
{
  public:
    ProcessorFamily(){};
    ~ProcessorFamily(){};
    virtual std::string getFamily() const = 0;
};

class RazorFamily : public ProcessorFamily
{
  public:
    std::string getFamily() const override
    {
        return "Razor";
    }
};

class RazorProFamily : public ProcessorFamily
{
  public:
    std::string getFamily() const override
    {
        return "Razor PRO";
    }
};

class AmdProcessor : public IProcessor
{
  public:
  protected:
    std::shared_ptr<ProcessorFamily> processorFamily;

  public:
    AmdProcessor();
    AmdProcessor(std::shared_ptr<ProcessorType> processorType, std::shared_ptr<ProcessorFamily> processorFamily)
        : IProcessor(processorType), processorFamily(processorFamily)
    {
    }


    void getComponentInfo() const override
    {
        std::cout << "AMD Processor with specs: "
                  << "family - " << processorFamily->getFamily() << "; type - " << getProcessorType() << std::endl;
    }
};

class ProcessorBranding
{
  public:
    ProcessorBranding(){};
    ~ProcessorBranding(){};
    virtual std::string getBranding() const = 0;
};

class I8Branding : public ProcessorBranding
{
  public:
    std::string getBranding() const override
    {
        return "I8";
    }
};

class I9Branding : public ProcessorBranding
{
  public:
    std::string getBranding() const override
    {
        return "I9";
    }
};

class IntelProcessor : public IProcessor
{

  protected:
    std::shared_ptr<ProcessorBranding> processorBranding;

  public:
    IntelProcessor(){};
    IntelProcessor(std::shared_ptr<ProcessorType> processorType, std::shared_ptr<ProcessorBranding> processorBranding)
        : IProcessor(processorType), processorBranding(processorBranding)
    {
    }

    void getComponentInfo() const override
    {
        std::cout << "Intel Processor with specs: "
                  << "branding - " << processorBranding->getBranding() << "; type - " << getProcessorType() << std::endl;
    }
};

class IRam : public PcComponent
{
  public:
    enum DoubleDateRate
    {
        DDR3,
        DDR4,
        DDR5
    };

    IRam(){};
    ~IRam(){};

    IRam(std::shared_ptr<DoubleDateRate> ddr) : ddr(ddr)
    {
    }

    std::string getDdr() const
    {
        switch (*ddr)
        {
        case DoubleDateRate::DDR3:
            return "DDR3";
        case DoubleDateRate::DDR4:
            return "DDR4";
        case DoubleDateRate::DDR5:
            return "DDR5";
        default:
            return "Not set or unknown";
        }
    }

  protected:
    std::shared_ptr<DoubleDateRate> ddr;
};

class KingstonRam : public IRam
{
  public:
    KingstonRam(std::shared_ptr<DoubleDateRate> ddr) : IRam(ddr)
    {
    }

    void getComponentInfo() const override
    {
        std::cout << "Kingston RAM with specs: " << getDdr() << std::endl;
    }
};

class ApacerRam : public IRam
{
  public:
    ApacerRam(std::shared_ptr<DoubleDateRate> ddr) : IRam(ddr)
    {
    }

    void getComponentInfo() const override
    {
        std::cout << "Apacer RAM with specs: " << getDdr() << std::endl;
    }
};

class Computer
{
  private:
    std::shared_ptr<IProcessor> processor;
    std::shared_ptr<IRam> ram;

  public:
    Computer(std::shared_ptr<IProcessor> processor, std::shared_ptr<IRam> ram) : processor(processor), ram(ram)
    {
    }

    void Specs()
    {
        processor->getComponentInfo();
        ram->getComponentInfo();
    }

};


