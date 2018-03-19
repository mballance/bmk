
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
