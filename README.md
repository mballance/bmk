
# BMK - The Bare Metal Kernel

BMK provides low-level services for system boot and managing execution, such as threading.

TODO:
- Need devicetree reader
- Need implementation that retrieves num_procs from devicetree
- Will users want/need access to devicetree?


Implementation
- atomics
- sys
  - Contains system-specific boot code
  - pthread 
- context -- contains primitives for storing thread context
  - 
  
## libfemto
libfemto source is obtained from http://github.com/michaeljclark/riscv-probe.git, a project with some simple bare-metal tests for RISCV platforms. The source is licensed under a permissive license.

# Startup Procedure

On startup, platform-specific boot code must determine which is the primary CPU
- The primary CPU must initialize the memory image, perform relocations, etc
- The non-primary CPUs (if any) must enter an idle loop waiting to be awakened

## Level0 Startup

After initializing the memory image and configuring the stack, the primary CPU must call bmk_startup(). 
bmk_startup first calls bmk_hardware_init(), a weak function provided to allow platform-specific hardware
initialization.

bmk_startup next calls bmk_core0_init() prior to calling level0_main(). In some cases, core0_init will be
a nop. In other cases, core0_init will be responsible for loading the main program.

bmk_startup next calls bmk_level0_main, which is responsible for performing application initialization.
-> Initialize any additional memory image (for example, a test loaded during core0_init)
-> Configure each processor that should be active with a stack
-> (Optional) Configure a level1-main function
-> (Optional) Configure a bmk_main function

bmk_startup next notifies cores that have been configured and waits for them to wake up. 
As non-primary cores awaken, they call bmk_level1_main.
Once all configured cores have awakened, bmk_startp calls bmk_level1_main() for the primary core.

The default implementation of bmk_level1_main() is to call bmk_main() for the primary core.

