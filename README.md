# Mattock Opportunistic Hashing Library.
Simple (limited) C++ wrapper for the libb2 BLAKE2BP  and the Crypto++ SHA1 hash functionality.
This library is meant to be the opportunistic-hashing core for MattockFS, a user-space
forensic-computing file-system aiming to become an essential part of the underpinning of
an envisioned open computer forensics framework.

The library also tries to provide the envisioned framework with a convenient migrational path
away from SHA1 hashing algoritm that was recently proven to be practically broken up to the 
point where its continued use could soon become a serious issue to computer forensic processes.

The envisioned migrational path to an alternate hashing algoritm is the following:

* Legacy SHA1 mode
* Transitional mode using a concatenation of a BLAKE2BP hash and a SHA1 hash (about 20% loss of performance)
* Fast mode using just BLAKE2BP (about 300% increase in performance)

The library defines an opportunistic hash typedef for each of these transitional modes. 
You can create 3 types of instances from these types:

* An instance meant for use with existing known-size immutable data entities.
* An instance meant for new 'sparse' entities that are just being created.
* An instance meant for non-sparse mutable entities.

Every time a chunk of data is read, the 'read\_chunk' method of the opportunistic hashing object 
should be invoked with data, size and offset. Every time a chunk of data is written, the 'written\_chunk' 
method of the opportunistic hashing object should be invoked with data, size and offset.
The ohash object will try to move oportunistic hashing forward if possible or will start from scratch if nesisary. 
You may query the ohash object to find out the offset of the data that has not yet been hashed opportunistically.

Once this offset reaches the actual file-size, the ohash state can be completed by invoking 'done'.
For known-size immutable objects the use of 'done' isn't required as the ohash object will be able to determine
itself wether it is done computing the opportunistic hash of the immutable object.

The resulting hash can than be retreived as hexadecimal string value.
