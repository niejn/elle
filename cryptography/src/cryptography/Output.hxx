#ifndef INFINIT_CRYPTOGRAPHY_OUTPUT_HXX
# define INFINIT_CRYPTOGRAPHY_OUTPUT_HXX

/*-------------.
| Serializable |
`-------------*/

ELLE_SERIALIZE_SIMPLE(infinit::cryptography::Output,
                      archive,
                      value,
                      format)
{
  enforce(format == 0);

  archive & value._buffer;
}

#endif