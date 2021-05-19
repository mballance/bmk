
- Provide bare-bones execution environment
  - Startup with a single core
  - Managing bring-up of multiple cores
    - Exception stack
    - 
  - Multi-core synchronization
    - mutex, condition
    - spinlock- and IPI-based
      - Need call-outs for 
  - Basic slab-based allocation (?)
  - Per-core interrupts
    - Provide call-in for 
  - Abstraction of memory access
  - Basic message reporting

- Anti-requirements
  - No multi-core scheduler
  - No device-tree support
  
