# dev\_ Family of Logging Options

The dev\_\*() family of functions, such as dev_emerg(), dev_alert(), dev_crit(), dev_err(), dev_warn(), dev_notice(), dev_info() and dev_dbg() are the recommended logging functions for use in device drivers.

This module registers a simple platform device. It logs messages using dev_emerg(), dev_alert(), dev_crit(), dev_err(), dev_warn(), dev_notice(), dev_info(), and dev_dbg(). It uses KBUILD_MODNAME for module identification in a platform_device to demonstrate real-world device driver logging.
