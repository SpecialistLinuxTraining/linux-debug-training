This module demonstrates how different kernel logging levels work using both printk() and pr\_\*() functions.

printk(KERN*\*) and pr*\_() provide similar functionality, but pr\_\_() is recommended for clarity and maintainability.

Higher priority messages (KERN_EMERG, KERN_ALERT) appear in system logs immediately.

Lower priority logs (KERN_DEBUG) may be filtered unless debug mode is enabled.
