#define QS_EXPORT __attribute__((visibility("default")))

#ifdef __cplusplus
#define QS_EXPORT_FUNC extern "C" QS_EXPORT
#else
#define QS_EXPORT_FUNC QS_EXPORT
#endif