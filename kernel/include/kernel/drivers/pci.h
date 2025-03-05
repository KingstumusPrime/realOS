#ifndef _KERNEL_PCI_H
#define _KERNEL_PCI_H
#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC
unsigned short pci_config_read_word(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset);
void pci_config_write_word(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset, unsigned short data);
void check_function(unsigned char bus, unsigned char device, unsigned char function );
void check_device(unsigned char bus, unsigned char device);
unsigned short get_vendor_id(unsigned char bus, unsigned char slot, unsigned char func );
unsigned short get_base_class(unsigned char bus, unsigned char slot, unsigned char func );
unsigned short get_sub_class(unsigned char bus, unsigned char slot, unsigned char func );
unsigned short get_secondary_bus(unsigned char bus, unsigned char slot, unsigned char func );
unsigned short get_prog_lf(unsigned char bus, unsigned char slot, unsigned char func );
unsigned short get_header_type(unsigned char bus, unsigned char slot, unsigned char func);
void pci_add_device(unsigned char bus, unsigned char device, unsigned char function);
void check_bus(unsigned char bus);

void check_all_buses(void);
typedef struct pci_device_t pci_device_t;
struct pci_device_t {
    unsigned short bus;
    unsigned short device;
    unsigned short function;
    unsigned short baseclass;
    unsigned short subclass;
    unsigned short lf;
    pci_device_t* next;
};

extern pci_device_t* pci_dev_root;

#endif