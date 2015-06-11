#ifndef INFINIT_CRYPTOGRAPHY_RSA_PUBLICKEY_HH
# define INFINIT_CRYPTOGRAPHY_RSA_PUBLICKEY_HH

# include <cryptography/fwd.hh>
# include <cryptography/types.hh>
# include <cryptography/Code.hh>
# include <cryptography/Clear.hh>
# include <cryptography/Oneway.hh>
# include <cryptography/Cipher.hh>
# include <cryptography/rsa/Seed.hh>
# include <cryptography/rsa/Padding.hh>
# include <cryptography/rsa/defaults.hh>

# include <elle/concept/Uniquable.hh>
# include <elle/types.hh>
# include <elle/attribute.hh>
# include <elle/operator.hh>
# include <elle/serialize/fwd.hh>
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
      /// Represent a public key in the RSA asymmetric cryptosystem.
      class PublicKey:
        public elle::Printable,
        public elle::concept::MakeUniquable<PublicKey>
      {
        /*-------------.
        | Construction |
        `-------------*/
      public:
        PublicKey(); // XXX[to deserialize]
        /// Construct a public key out of its private counterpart.
        explicit
        PublicKey(PrivateKey const& k,
                  Cipher const envelope_cipher,
                  Mode const envelope_mode);
        /// Construct a public key based on the given EVP_PKEY key whose
        /// ownership is transferred.
        explicit
        PublicKey(::EVP_PKEY* key,
                  Padding const encryption_padding,
                  Padding const signature_padding,
                  Oneway const digest_algorithm,
                  Cipher const envelope_cipher,
                  Mode const envelope_mode);
        /// Construct a public key based on the given RSA key whose
        /// ownership is transferred to the public key.
        explicit
        PublicKey(::RSA* rsa,
                  Padding const encryption_padding,
                  Padding const signature_padding,
                  Oneway const digest_algorithm,
                  Cipher const envelope_cipher,
                  Mode const envelope_mode);
# if defined(INFINIT_CRYPTOGRAPHY_ROTATION)
        /// Construct a public key based on a given seed i.e in a deterministic
        /// way.
        explicit
        PublicKey(Seed const& seed,
                  Padding const encryption_padding =
                    defaults::encryption_padding,
                  Padding const signature_padding =
                    defaults::signature_padding,
                  Oneway const digest_algorithm =
                    defaults::digest_algorithm,
                  Cipher const envelope_cipher =
                    defaults::envelope_cipher,
                  Mode const envelope_mode =
                    defaults::envelope_mode);
# endif
        PublicKey(PublicKey const& other);
        PublicKey(PublicKey&& other);
        ELLE_SERIALIZE_CONSTRUCT_DECLARE(PublicKey);
        virtual
        ~PublicKey() = default;

        /*--------.
        | Methods |
        `--------*/
      private:
        /// Construct the object based on the given RSA structure whose
        /// ownership is transferred to the callee.
        void
        _construct(::RSA* rsa);
        /// Prepare the public key cryptographic contexts.
        void
        _prepare();
        /// Check that the key is valid.
        void
        _check() const;
      public:
        /// Encrypt any serializable type, including Plain.
        template <typename T = Plain>
        Code
        encrypt(T const& value) const;
        /// Return true if the given signature matches with the digest.
        elle::Boolean
        verify(Signature const& signature,
               Digest const& digest) const;
        /// Return true if the given signature matches with the serializable
        /// value.
        template <typename T = Plain>
        elle::Boolean
        verify(Signature const& signature,
               T const& value) const;
        /// Decrypt the given code.
        ///
        /// Although unusual, the public key can very well be used for
        /// decrypting in which case the private key would be used for
        /// encrypting.
        Clear
        decrypt(Code const& code) const;
# if defined(INFINIT_CRYPTOGRAPHY_ROTATION)
        /// Return the seed once rotated by the public key.
        Seed
        rotate(Seed const& seed) const;
        /// Return the seed once unrotated by the public key.
        Seed
        unrotate(Seed const& seed) const;
# endif
        /// Return the public key's size in bytes.
        elle::Natural32
        size() const;
        /// Return the public key's length in bits.
        elle::Natural32
        length() const;

        /*----------.
        | Operators |
        `----------*/
      public:
        elle::Boolean
        operator ==(PublicKey const& other) const;
        elle::Boolean
        operator <(PublicKey const& other) const;
        ELLE_OPERATOR_NO_ASSIGNMENT(PublicKey);

        /*----------.
        | Printable |
        `----------*/
      public:
        void
        print(std::ostream& stream) const override;

        /*-------------.
        | Serializable |
        `-------------*/
      public:
        ELLE_SERIALIZE_FRIEND_FOR(PublicKey);

        /*--------------.
        | Serialization |
        `--------------*/
      public:
        PublicKey(elle::serialization::SerializerIn& serializer);
        void
        serialize(elle::serialization::Serializer& serializer);

        /*-----------.
        | Attributes |
        `-----------*/
      public:
        ELLE_ATTRIBUTE_R(types::EVP_PKEY, key);
        ELLE_ATTRIBUTE_R(Padding, encryption_padding);
        ELLE_ATTRIBUTE_R(Padding, signature_padding);
        ELLE_ATTRIBUTE_R(Oneway, digest_algorithm);
        ELLE_ATTRIBUTE_R(Cipher, envelope_cipher);
        ELLE_ATTRIBUTE_R(Mode, envelope_mode);
        // Note that the contexts are not serialized because they can
        // be reconstructed out of the paddings and algorithms above.
        struct
        {
          types::EVP_PKEY_CTX encrypt;
          types::EVP_PKEY_CTX verify;
# if defined(INFINIT_CRYPTOGRAPHY_ROTATION)
          types::EVP_PKEY_CTX rotate;
          types::EVP_PKEY_CTX unrotate;
# endif
          // The encryption padding size expressed in bits.
          elle::Natural32 envelope_padding_size;
        } _context;
      };
    }
  }
}

# include <cryptography/rsa/PublicKey.hxx>

#endif
