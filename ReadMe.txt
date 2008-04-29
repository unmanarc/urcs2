URCS is a remote control tool with a Windows based client and Linux based server. The tool provides a shell to the user and can be configured from an HTTP URL. The tool also support scripting language for customizable actions.

URCS 2.13 Current features:

- Shell based control, Shell/Remote based installation
- Reverse proxy system (Avoid NAT/PAT IP Address shortage)
- Keep-alive system protection
- TCP redirection. (multiple and reverse pipes...)
- Secure password interchange: (md5(md5(key)+random_seed))
- Installation over net, (Service, HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER) - Configuration over URL (Any configuration parameter can be obtained from a URL)
- User-groups configurable based system
- Scripting language
- Server is compatible with wine.

Functions:

- Network Sockets management functions (net_*)
- Proxy/bouncing functions (prx_*)
- Registry Manipulation (reg_*)
- Console commands
- File manipulation commands
- Process manipulation commands (ps,kill,killall)
- System manipulation commands 

WARNING:

- This tool is unmantained since 2004.
- It was developed using Visual Studio 2002, and maybe does not compile with newer compilers.
- No binaries are available.
- core/proxy servers are unavalable. use yours.

CRITICAL WARNING:

In 2004 encrypted communications were not the norm. This tool was intended to replace telnet (a non-encrypted communication protocol) 
by sending line-to-line instead byte-to-byte and reducing 256Kbps communication lag (2004). Therefore, the encryption mechanism 
was very trivial and easy to eavasdrop, and it's not intended to replace SSL/TLS or SSH protocols. 

Hereby, I do not recommend to use this tool anymore until TLS/SSL or another well accepted cipher option is implemented.

This source code is available only for reference and educational purporses.
