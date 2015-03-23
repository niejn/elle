#ifndef INFINIT_CRYPTOGRAPHY_RSA_PRIVATEKEY_HH
# define INFINIT_CRYPTOGRAPHY_RSA_PRIVATEKEY_HH

# include <cryptography/fwd.hh>
# include <cryptography/types.hh>
# include <cryptography/Clear.hh>
# include <cryptography/Signature.hh>
# include <cryptography/oneway.hh>
# include <cryptography/rsa/Seed.hh>

# include <elle/types.hh>
# include <elle/attribute.hh>
# include <elle/operator.hh>
# include <elle/serialize/construct.hh>

# include <utility>
ELLE_OPERATOR_RELATIONALS();

# include <openssl/evp.h>

//
// ---------- Class -----------------------------------------------------------
//

namespace infinit
{
  namespace cryptography
  {
    namespace rsa
    {
      /// Represent a private key in the RSA asymmetric cryptosystem.
      class PrivateKey:
        public elle::Printable
      {
        /*-------------.
        | Construction |
        `-------------*/
      public:
        PrivateKey(); // XXX[deserialize]
        /// Construct a private key based on the given EVP_PKEY key whose
        /// ownership is transferred.
        explicit
        PrivateKey(::EVP_PKEY* key);
        /// Construct a private key based on the given RSA key whose
        /// ownership is transferred to the private key.
        explicit
        PrivateKey(::RSA* rsa);
        /// Construct a private key by transferring ownership of some big
        /// numbers.
        PrivateKey(::BIGNUM* n,
                   ::BIGNUM* e,
                   ::BIGNUM* d,
                   ::BIGNUM* p,
                   ::BIGNUM* q,
                   ::BIGNUM* dmp1,
                   ::BIGNUM* dmq1,
                   ::BIGNUM* iqmp);
# if defined(INFINIT_CRYPTOGRAPHY_ROTATION)
        /// Construct a private key based on a given seed i.e in a deterministic
        /// way.
        explicit
        PrivateKey(Seed const& seed);
# endif
        PrivateKey(PrivateKey const& other);
        PrivateKey(PrivateKey&& other);
        ELLE_SERIALIZE_CONSTRUCT_DECLARE(PrivateKey);

        /*--------.
        | Methods |
        `--------*/
      private:
        /// Construct the object based on the given RSA structure whose
        /// ownership is transferred to the callee.
        void
        _construct(::RSA* rsa);
        /// Construct the object based on big numbers.
        ///
        /// Note that when called, the number are already allocated for the
        /// purpose of the object construction. In other words, the ownership
        /// is transferred to the private key being constructed. Thus, it is
        /// the responsibility of the private key being constructed to release
        /// memory should an error occur, i.e not the caller's
        void
        _construct(::BIGNUM* n,
                   ::BIGNUM* e,
                   ::BIGNUM* d,
                   ::BIGNUM* p,
                   ::BIGNUM* q,
                   ::BIGNUM* dmp1,
                   ::BIGNUM* dmq1,
                   ::BIGNUM* iqmp);
        /// Prepare the private key cryptographic contexts.
        void
        _prepare();
      public:
        /// Decrypt a code and returns the original clear text.
        ///
        /// Note that the code is, in practice, an archive containing both
        /// a temporarily-generated secret key and the plain text encrypted
        /// with the secret key.
        Clear
        decrypt(Code const& code) const;
        /// Decrypt a code and returns the given type, assuming the given type
        /// can be extracted from the clear, which should then be an archive.
        template <typename T>
        T
        decrypt(Code const& code) const;
        /// Return a signature of the given digest.
        Signature
        sign(Digest const& digest) const;
        /// Return a signature of the given plain text.
        Signature
        sign(Plain const& plain) const;
        /// Return a signature of any given serializable type.
        template <typename T>
        Signature
        sign(T const& value) const;
# if defined(INFINIT_CRYPTOGRAPHY_ROTATION)
        /// Return the seed once derived by the private key.
        Seed
        derive(Seed const& seed) const;
        /// Return the seed once rotated by the private key.
        Seed
        rotate(Seed const& seed) const;
# endif
        /// Return the private key's size in bytes.
        elle::Natural32
        size() const;
        /// Return the private key's length in bits.
        elle::Natural32
        length() const;

        /*----------.
        | Operators |
        `----------*/
      public:
        elle::Boolean
        operator ==(PrivateKey const& other) const;
        elle::Boolean
        operator <(PrivateKey const& other) const;
        ELLE_OPERATOR_NO_ASSIGNMENT(PrivateKey);

        /*----------.
        | Printable |
        `----------*/
      public:
        void
        print(std::ostream& stream) const;

        /*----------.
        | Serialize |
        `----------*/
      public:
        ELLE_SERIALIZE_FRIEND_FOR(PrivateKey);

        /*-------------.
        | Serializable |
        `-------------*/
      public:
        PrivateKey(elle::serialization::SerializerIn& serializer);
        void
        serialize(elle::serialization::Serializer& serializer);

        /*-----------.
        | Attributes |
        `-----------*/
      private:
        ELLE_ATTRIBUTE_R(types::EVP_PKEY, key);
        ELLE_ATTRIBUTE(types::EVP_PKEY_CTX, context_decrypt);
        ELLE_ATTRIBUTE(types::EVP_PKEY_CTX, context_sign);
# if defined(INFINIT_CRYPTOGRAPHY_ROTATION)
        ELLE_ATTRIBUTE(types::EVP_PKEY_CTX, context_derive);
        ELLE_ATTRIBUTE(types::EVP_PKEY_CTX, context_rotate);
# endif
      };
    }
  }
}

# include <cryptography/rsa/PrivateKey.hxx>

#endif
