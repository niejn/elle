#ifndef ELLE_IO_PATH_HXX
# define ELLE_IO_PATH_HXX

# include <elle/standalone/Report.hh>

# include <elle/idiom/Open.hh>

namespace elle
{
  namespace io
  {

//
// ---------- variadic templates ----------------------------------------------
//

    ///
    /// this template resolves a single piece of the path's pattern.
    ///
    template <typename T>
    Status              Path::Complete(T                        piece)
    {
      // resolve the piece.
      if (this->Complete(piece.name, piece.value) == Status::Error)
        escape("unable to resolve the piece");

      return Status::Ok;
    }

    ///
    /// this method resolves the given set of pieces for the path's pattern.
    ///
    template <typename T,
              typename... TT>
    Status              Path::Complete(T                        piece,
                                       TT...                    pieces)
    {
      // resolve the given piece.
      if (this->Complete(piece) == Status::Error)
        escape("unable to resolve the first piece");

      // resolve the additional pieces.
      if (this->Complete(pieces...) == Status::Error)
        escape("unable to resolve the additional pieces");

      return Status::Ok;
    }

  }
}

#endif
