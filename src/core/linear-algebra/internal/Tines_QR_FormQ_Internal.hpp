#ifndef __TINES_QR_FORM_Q_INTERNAL_HPP__
#define __TINES_QR_FORM_Q_INTERNAL_HPP__

#include "Tines_ApplyQ_Internal.hpp"
#include "Tines_Internal.hpp"
#include "Tines_Set_Internal.hpp"

namespace Tines {

  struct QR_FormQ_Internal {
    template <typename MemberType, typename ValueType>
    KOKKOS_INLINE_FUNCTION static int
    invoke(const MemberType &member, const int m, const int k,
           /* */ ValueType *A, const int as0, const int as1,
           /* */ ValueType *t, const int ts,
           /* */ ValueType *Q, const int qs0, const int qs1,
           /* */ ValueType *w, const bool is_Q_zero = false) {
      using value_type = ValueType;

      /// Given a matrix A that includes QR factorization
      /// it forms a unitary matrix Q
      ///   B = Q = (H0 H1 H2 H3 ... H(k-1)) I
      /// where
      ///   A is m x k (holding H0, H1 ... H(k-1)
      ///   t is k x 1
      ///   B is m x m

      // set identity
      const value_type one(1), zero(0);
      if (is_Q_zero)
        SetInternal::invoke(member, m, one, Q, qs0 + qs1);
      else
        SetInternal::invoke(member, m, m, one, zero, Q, qs0, qs1);
      member.team_barrier();

      return ApplyQ_LeftForwardInternal ::invoke(member, m, m, k, A, as0, as1,
                                                 t, ts, Q, qs0, qs1, w);
    }
  };

} // namespace Tines

#endif
