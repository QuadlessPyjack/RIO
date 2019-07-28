h1 { margin-bottom: 0.21cm; background: transparent none repeat scroll 0% 0%; page-break-after: avoid; }h1.western { font-family: "Liberation Sans", sans-serif; font-size: 18pt; font-weight: bold; }h1.cjk { font-family: "Liberation Sans"; font-size: 18pt; font-weight: bold; }h1.ctl { font-family: "Noto Sans Devanagari"; font-size: 18pt; font-weight: bold; }p { margin-bottom: 0.25cm; line-height: 115%; background: transparent none repeat scroll 0% 0%; }

**RIO
SSH Client-Server Demo**

## Contents

- RIO_Client:
	  
    - Qt QML GUI Client that connects via SSH to a server. 

- RIO_Server 
    - Qt headless server that accepts SSH connections. 
		 
    - Certificate authentication is performed server-side (some
      preliminary work for supporting multiple clients is there but
      currently only one client at a time can connect) 
    - DB Connector for logging incoming messages is for MySQL,
      tested with MariaDB on Gentoo without issues. Shouldn’t be any
      difference. 

- RIO_protocol 
    - Provides the data struct used for sending data. 
    - Provides helper functions to handle the data struct
      (serialize/deserialize etc.) 

- certs 
    - Dummy certs used to test the SSH connection 
    - Certs are associated with 127.0.0.1 for both the client and
      the server 
    - The applications themselves embed the certificates in
      .qrc resource files to prevent tampering (storing them encrypted
      would be nice) 

NOTES:

	Built with Qt 5.12.3 using QtCreator 4.8.2

	Development platform is x86-64 Gentoo Linux.

	No platform-dependent code was used so running on Windows should
        simply be a matter of changing kits and rebuilding the project.

	Database connection notes:

	Connection details are in
        RIO_Server/persistence.cpp and match the database dump found in
	./riodb-dump.sql
