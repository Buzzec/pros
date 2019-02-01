#include "kapi.h"
#include "system/hot.h"


// stored only in cold
struct hot_table __HOT_TABLE = { 0 };
struct hot_table* const HOT_TABLE = &__HOT_TABLE;

#define MAGIC0 0x52616368
#define MAGIC1 0x8CEF7310

__attribute__((section (".hot_magic"))) uint32_t MAGIC[] = {MAGIC0, MAGIC1};
uint32_t const volatile * const MAGIC_ADDR = MAGIC;

// The linker decides on these symbols in each section just as normal
// When linking in hot, these pointers work just like any other weak symbol
// Note: to get C++ style initialize and friends, we strip out cpp_initialize and friends so that linker
// regenerates that function with the call to the correct (user-written) C++ version
extern char const* _PROS_COMPILE_TIMESTAMP;
extern char const* _PROS_COMPILE_DIRECTORY;

void __libc_init_array();
uint8_t* __sbss_start;
uint8_t* __sbss_end;
uint8_t* __bss_start;
uint8_t* __bss_end;

// this expands to a bunch of:
// extern void autonomous();
#define FUNC(F) void F();
#include "system/user_functions/list.h"
#undef FUNC

__attribute__((section (".hot_init")))
void install_hot_table(struct hot_table* const tbl) {
  printf("Hot initializing\n");
  tbl->compile_timestamp = _PROS_COMPILE_TIMESTAMP;
  tbl->compile_directory = _PROS_COMPILE_DIRECTORY;

  // this expands to a bunch of:
  // tbl->functions.autonomous = autonomous;
  #define FUNC(F) tbl->functions.F = F;
  #include "system/user_functions/list.h"
  #undef FUNC

  // Zero fill the sbss and bss segments.
  memset(__sbss_start, 0, __sbss_end - __sbss_start);
  memset(__bss_start, 0, __bss_end - __bss_end);

  __libc_init_array();
}

void invoke_install_hot_table() {
  printf("%s %p %p %x %x\n", __FUNCTION__, (void*)install_hot_table, (void*)HOT_TABLE, MAGIC_ADDR[0], MAGIC_ADDR[1]);
  if(MAGIC_ADDR[0] == MAGIC0 && MAGIC_ADDR[1] == MAGIC1) {
    install_hot_table(HOT_TABLE);
  } else {
    memset(HOT_TABLE, 0, sizeof(*HOT_TABLE));
  }
}
