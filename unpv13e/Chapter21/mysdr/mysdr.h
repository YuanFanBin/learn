#ifndef __mysdr_h
#define __mysdr_h

#define SAP_VERSION                     1
#define SAP_VERSION_MASK        0xe0000000
#define SAP_VERSION_SHIFT       29
#define SAP_IPV6                        0x10000000
#define SAP_DELETE                      0x04000000
#define SAP_ENCRYPTED           0x02000000
#define SAP_COMPRESSED          0x01000000
#define SAP_AUTHLEN_MASK        0x00ff0000
#define SAP_AUTHLEN_SHIFT       16
#define SAP_HASH_MASK           0x0000ffff

#endif // __mysdr_h
