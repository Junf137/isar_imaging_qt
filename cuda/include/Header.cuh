//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"

#ifdef __cpluscplus
extern "C"
{
#endif

#define DLL_EXPORT_API __declspec(dllexport)

//DLL_EXPORT_API cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size);
DLL_EXPORT_API int arrayAdd(int* c, const int* a, const int* b, const unsigned int size);

#ifdef __cpluscplus
}
#endif // __cpluscplus