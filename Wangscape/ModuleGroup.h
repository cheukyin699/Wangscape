#pragma once
#include <noise/noise.h>
#include <string>
#include <map>
#include <memory>

// Maybe these typedefs should be elsewhere?
typedef noise::module::Module Module;
typedef std::shared_ptr<Module> ModulePtr;

// A helper class to store related noise modules in one place.
//
// Each component module is identified with a std::string.
// The module identified by OUTPUT_MODULE ("output") is
// used in calls to GetValue(x,y,z).
// Component modules are stored in std::shared_ptrs,
// so they may be reused in other ModuleGroups.
// Note that libnoise's internal pointers to source modules
// are all raw pointers, which do not have ownership,
// cannot be used to save a ModulePtr from deletion,
// and must never be used to delete a module stored
// in a ModulePtr.
// A module in a ModuleGroup may have a source module
// which is not managed by the ModuleGroup,
// but the ModuleGroup will not be responsible
// for its deletion, and will be vulnerable to crashes
// if the unmanaged source module is deleted.
class ModuleGroup : public noise::module::Module
{
public:
    typedef std::map<std::string, ModulePtr> ModuleContainer;

    ModuleContainer modules;
    const static std::string OUTPUT_MODULE;

    ModuleGroup();
    ~ModuleGroup();
    virtual int GetSourceModuleCount() const;
    virtual double GetValue(double x, double y, double z) const;
    
};

