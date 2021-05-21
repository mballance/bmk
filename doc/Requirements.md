
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
  

# Board functions
- Send a software interrupt to a specific core
  - bmk_sys_send_proc_event() - Send interrupt to selected processors
  - bmk_sys_wait_proc_event() - 
- Handle interrupt
  - Delegate to a BMK function specific to handling software interrupt
    - BMK itself must be aware of software interrupts (?)
      - Unclear whether this is true. Board-support may be able to handle on its own
        since it needs to get involved
  - Delegate to a BMK function specific to handling non-software interrupt
    - User code wants to be aware of non-software interrupts

  