#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "../blackbone/src/BlackBone/Process.h"
#include "../blackbone/src/BlackBone/RemoteFunction.hpp"
#include "../blackbone/src/BlackBone/Utils.h"

wchar_t* GetCmdOption(wchar_t ** begin, wchar_t ** end, const std::wstring & option) {
  wchar_t ** itr = find(begin, end, option);
  if (itr != end && ++itr != end) {
    return *itr;
  }
  return 0;
}

bool CmdOptionExists(wchar_t** begin, wchar_t** end, const std::wstring& option) {
  return find(begin, end, option) != end;
}

bool FileExists(const std::wstring& name) {
  std::ifstream f(name);
  if (f.good()) {
    f.close();
    return true;
  }
  else {
    f.close();
    return false;
  }
}

std::vector<char> FileReadAllBytes(const std::wstring& name) {
  std::ifstream input(name, std::ios::binary);
  if (input.is_open()) {

    std::vector<char> buffer((
      std::istreambuf_iterator<char>(input)),
      (std::istreambuf_iterator<char>()));

    return buffer;
  }
  return std::vector<char>();
}

int ExecuteGetDomain(blackbone::Process& process) {
  typedef int(__cdecl*  mono_domain_get) ();

  auto mono_get_root_domain_address = process.modules().GetExport(process.modules().GetModule(L"mono.dll"), "mono_domain_get");
  if (mono_get_root_domain_address.procAddress == 0) {
    std::wcout << L"Could not find mono_domain_get!";
    return 0;
  }

  blackbone::RemoteFunction<mono_domain_get> mono_domain_get_function(process, (mono_domain_get)mono_get_root_domain_address.procAddress);

  int domain_result;
  mono_domain_get_function.Call(domain_result, process.threads().getMain());

  return domain_result;
}
int ExecuteImageOpenFromDataFull(blackbone::Process& process, std::vector<char>& data) {
  typedef int(__cdecl* mono_image_open_from_data_full) (char *data, unsigned int data_len, int need_copy, int *status, int refonly);

  auto mono_image_open_from_data_full_address = process.modules().GetExport(process.modules().GetModule(L"mono.dll"), "mono_image_open_from_data");
  if (mono_image_open_from_data_full_address.procAddress == 0) {
    std::wcout << L"Could not find mono_image_open_from_data_full!";
    return 0;
  }

  int status;
  blackbone::RemoteFunction<mono_image_open_from_data_full> mono_image_open_from_data_full_function(process, (mono_image_open_from_data_full)mono_image_open_from_data_full_address.procAddress, data.data(), data.size(), 1, &status, 0);

  int image_data_get_result;
  mono_image_open_from_data_full_function.setArg(0, blackbone::AsmVariant(data.data(), data.size()));
  mono_image_open_from_data_full_function.Call(image_data_get_result, process.threads().getMain());

  return image_data_get_result;
}
int ExecuteAssemblyLoadFromFull(blackbone::Process& process, int image) {
  typedef int(__cdecl* mono_assembly_load_from_full) (int image, int *fname, int *status, bool refonly);

  auto mono_assembly_load_from_full_address = process.modules().GetExport(process.modules().GetModule(L"mono.dll"), "mono_assembly_load_from_full");
  if (mono_assembly_load_from_full_address.procAddress == 0) {
    std::wcout << L"Could not find mono_assembly_load_from_full!";
    return 0;
  }

  int status;
  blackbone::RemoteFunction<mono_assembly_load_from_full> mono_assembly_load_from_full_function(process, (mono_assembly_load_from_full)mono_assembly_load_from_full_address.procAddress, image, nullptr, &status, 0);

  int assembly;
  mono_assembly_load_from_full_function.Call(assembly, process.threads().getMain());

  return assembly;
}
int ExecuteAssemblyGetImage(blackbone::Process& process, int assembly) {
  typedef int(__cdecl*  mono_assembly_get_image) (int assembly);

  auto mono_assembly_get_image_address = process.modules().GetExport(process.modules().GetModule(L"mono.dll"), "mono_assembly_get_image");
  if (mono_assembly_get_image_address.procAddress == 0) {
    std::wcout << L"Could not find mono_assembly_get_image!";
    return 0;
  }

  blackbone::RemoteFunction<mono_assembly_get_image> mono_assembly_get_image_function(process, (mono_assembly_get_image)mono_assembly_get_image_address.procAddress, assembly);

  int image;
  mono_assembly_get_image_function.Call(image, process.threads().getMain());

  return image;
}
int ExecuteGetClassFromName(blackbone::Process& process, int image, const char* name_space, const char* name) {
  typedef int(__cdecl*  mono_class_from_name) (int image, const char* name_space, const char *name);

  auto mono_class_from_name_address = process.modules().GetExport(process.modules().GetModule(L"mono.dll"), "mono_class_from_name");
  if (mono_class_from_name_address.procAddress == 0) {
    std::wcout << L"Could not find mono_class_from_name!";
    return 0;
  }

  blackbone::RemoteFunction<mono_class_from_name> mono_class_from_name_function(process, (mono_class_from_name)mono_class_from_name_address.procAddress, image, name_space, name);

  int klass;
  mono_class_from_name_function.Call(klass, process.threads().getMain());

  return klass;
}
int ExecuteGetMethodFromName(blackbone::Process& process, int klass, const char* name) {
  typedef int(__cdecl*  mono_class_get_method_from_name) (int klass, const char *name, int param_count);

  auto mono_class_get_method_from_name_address = process.modules().GetExport(process.modules().GetModule(L"mono.dll"), "mono_class_get_method_from_name");
  if (mono_class_get_method_from_name_address.procAddress == 0) {
    std::wcout << L"Could not find mono_class_get_method_from_name!";
    return 0;
  }

  blackbone::RemoteFunction<mono_class_get_method_from_name> mono_class_get_method_from_name_function(process, (mono_class_get_method_from_name)mono_class_get_method_from_name_address.procAddress, klass, name, 0);

  int method;
  mono_class_get_method_from_name_function.Call(method, process.threads().getMain());

  return method;
}
int ExecuteRuntimeInvoke(blackbone::Process& process, int method) {
  typedef int(__cdecl*   mono_runtime_invoke) (int method, void *obj, void **params, int **exc);

  auto mono_runtime_invoke_address = process.modules().GetExport(process.modules().GetModule(L"mono.dll"), "mono_runtime_invoke");
  if (mono_runtime_invoke_address.procAddress == 0) {
    std::wcout << L"Could not find mono_runtime_invoke!";
    return 0;
  }

  blackbone::RemoteFunction<mono_runtime_invoke> mono_runtime_invoke_function(process, (mono_runtime_invoke)mono_runtime_invoke_address.procAddress, method, nullptr, nullptr, nullptr);

  int invoke_result;
  mono_runtime_invoke_function.Call(invoke_result, process.threads().getMain());

  return invoke_result;
}

int UseAssembly(blackbone::Process& process, std::wstring dll,  std::wstring name_space, std::wstring class_name, std::wstring method_name) {
  int domain = ExecuteGetDomain(process);
  if (!domain) {
    std::wcerr << "ExecuteGetDomain returned 0!" << std::endl;
    return 1;
  }

  auto data = FileReadAllBytes(dll);

  if (data.size() == 0) {
    std::wcerr << "FileReadAllBytes returned size = 0!" << std::endl;
    return 1;
  }

  int raw_image = ExecuteImageOpenFromDataFull(process, data);

  if (!raw_image) {
    std::wcerr << "ExecuteImageOpenFromDataFull returned 0!" << std::endl;
    return 1;
  }

  int assembly = ExecuteAssemblyLoadFromFull(process, raw_image);
  if (!assembly) {
    std::wcerr << "ExecuteAssemblyLoadFromFull returned 0!" << std::endl;
    return 1;
  }
  int image = ExecuteAssemblyGetImage(process, assembly);
  if (!assembly) {
    std::wcerr << "ExecuteAssemblyGetImage returned 0!" << std::endl;
    return 1;
  }
  int klass = ExecuteGetClassFromName(process, image, blackbone::Utils::WstringToUTF8(name_space).c_str(), blackbone::Utils::WstringToUTF8(class_name).c_str());
  if (!klass) {
    std::wcerr << "Searching for namespace: " << name_space << " with class: " << class_name << "." << std::endl;
    std::wcerr << "ExecuteGetClassFromName returned 0!" << std::endl;
    return 1;
  }

  int method = ExecuteGetMethodFromName(process, klass, blackbone::Utils::WstringToUTF8(method_name).c_str());

  if (!method) {
    std::wcerr << "Searching for method: " << method_name << std::endl;
    std::wcerr << "ExecuteGetMethodFromName returned 0!" << std::endl;
    return 1;
  }

  ExecuteRuntimeInvoke(process, method);

  std::wcout << "Execution sucessfull!" << std::endl;

  return 0;
}

int wmain(int argc, wchar_t * argv[]) {
  std::wcout << "MonoAssemblyInjector 0.1" << std::endl;
  std::wcout << "github.com/gamebooster/mono-assembly-booster" << std::endl << std::endl;

  if (argc < 11 || !CmdOptionExists(argv, argv + argc, L"-dll") || CmdOptionExists(argv, argv + argc, L"-h") || CmdOptionExists(argv, argv + argc, L"-help")) {
    std::wcout << "Example Usage: -dll assembly.dll -target hearthstone.exe -namespace MyProgram -class Loader -method Load" << std::endl << std::endl;
  }

  if (!CmdOptionExists(argv, argv + argc, L"-dll")) {
    std::wcerr << "You need to specify `-dll`!";
    return 1;
  }

  if (!CmdOptionExists(argv, argv + argc, L"-target")) {
    std::wcerr << "You need to specify `-target`!";
    return 1;
  }

  wchar_t * dll = GetCmdOption(argv, argv + argc, L"-dll");
  if (!dll) {
    std::wcerr << "You need to specify `-dll`!";
    return 1;
  }

  if (!FileExists(dll)) {
    std::wcerr << "Your specified dll does not exist!";
    return 1;
  }

  wchar_t * target = GetCmdOption(argv, argv + argc, L"-target");
  if (!target) {
    std::wcerr << "You need to specify `-target`!";
    return 1;
  }

  wchar_t * name_space = GetCmdOption(argv, argv + argc, L"-namespace");
  if (!name_space) {
    std::wcerr << "You need to specify `-namespace`!";
    return 1;
  }

  wchar_t * classname = GetCmdOption(argv, argv + argc, L"-class");
  if (!classname) {
    std::wcerr << "You need to specify `-class`!";
    return 1;
  }

  wchar_t * methodname = GetCmdOption(argv, argv + argc, L"-method");
  if (!methodname) {
    std::wcerr << "You need to specify `-method`!";
    return 1;
  }

  if (target) {
    blackbone::Process target_process;
    std::vector<DWORD> found;
    blackbone::Process::EnumByName(target, found);

    std::wcout << L"Searching for " << target << " ... " << std::endl;

    if (found.size() > 0) {
      if (target_process.Attach(found.front()) == STATUS_SUCCESS) {

        auto barrier = target_process.core().native()->GetWow64Barrier().type;

        if (barrier != blackbone::wow_32_32 && barrier != blackbone::wow_64_64)
        {
          std::wcout << L"Can't execute call through WOW64 barrier, aborting" << std::endl;
          return 1;
        }

        std::wcout << L"Found. Executing..." << std::endl;

        return UseAssembly(target_process,
                           dll,
                           name_space,
                           classname,
                           methodname);
      }
      else {
        std::wcout << L"Could not attach to " << target << "!" << std::endl;
        return 1;
      }
    }
    else {
      std::wcout << L"Could not found " << target << "!" << std::endl;
      return 1;
    }
  }

  return 0;
}