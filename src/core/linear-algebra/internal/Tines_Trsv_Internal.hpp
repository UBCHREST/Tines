#ifndef __TINES_TRSV_INTERNAL_HPP__
#define __TINES_TRSV_INTERNAL_HPP__

#include "Tines_Internal.hpp"

#include "Tines_Scale_Internal.hpp"
#include "Tines_Set_Internal.hpp"

namespace Tines {

  struct TrsvInternalLower {
    template <typename MemberType, typename ScalarType, typename ValueType>
    KOKKOS_INLINE_FUNCTION static int
    invoke(const MemberType &member, const bool use_unit_diag, const int m,
           const ScalarType alpha, const ValueType *__restrict__ A,
           const int as0, const int as1,
           /**/ ValueType *__restrict__ b, const int bs0) {
      const ScalarType one(1.0), zero(0.0);

      if (alpha == zero)
        SetInternal::invoke(member, m, zero, b, bs0);
      else {
        if (alpha != one)
          ScaleInternal::invoke(member, m, alpha, b, bs0);
        if (m <= 0)
          return 0;

        for (int p = 0; p < m; ++p) {
          const int iend = m - p - 1;

          const ValueType *__restrict__ a21 =
            iend ? A + (p + 1) * as0 + p * as1 : NULL;

          ValueType *__restrict__ beta1 = b + p * bs0,
                                  *__restrict__ b2 = iend ? beta1 + bs0 : NULL;

          member.team_barrier();
          ValueType local_beta1 = *beta1;
          if (!use_unit_diag) {
            const ValueType alpha11 = A[p * as0 + p * as1];
            local_beta1 = local_beta1 / alpha11;

            member.team_barrier();
            Kokkos::single(Kokkos::PerTeam(member),
                           [&]() { *beta1 = local_beta1; });
          }
          Kokkos::parallel_for(
            Kokkos::TeamVectorRange(member, 0, iend),
            [&](const int &i) { b2[i * bs0] -= a21[i * as0] * local_beta1; });
        }
      }
      return 0;
    }
  };

  struct TrsvInternalUpper {
    template <typename MemberType, typename ScalarType, typename ValueType>
    KOKKOS_INLINE_FUNCTION static int
    invoke(const MemberType &member, const bool use_unit_diag, const int m,
           const ScalarType alpha, const ValueType *__restrict__ A,
           const int as0, const int as1,
           /**/ ValueType *__restrict__ b, const int bs0) {
      const ScalarType one(1.0), zero(0.0);

      if (alpha == zero)
        SetInternal::invoke(member, m, zero, b, bs0);
      else {
        if (alpha != one)
          ScaleInternal::invoke(member, m, alpha, b, bs0);
        if (m <= 0)
          return 0;

        ValueType *__restrict__ b0 = b;
        for (int p = (m - 1); p >= 0; --p) {
          const int iend = p;

          const ValueType *__restrict__ a01 = A + p * as1;
          /**/ ValueType *__restrict__ beta1 = b + p * bs0;

          member.team_barrier();
          ValueType local_beta1 = *beta1;
          if (!use_unit_diag) {
            const ValueType alpha11 = A[p * as0 + p * as1];
            local_beta1 = local_beta1 / alpha11;

            member.team_barrier();
            Kokkos::single(Kokkos::PerTeam(member),
                           [&]() { *beta1 = local_beta1; });
          }
          Kokkos::parallel_for(
            Kokkos::TeamVectorRange(member, 0, iend),
            [&](const int &i) { b0[i * bs0] -= a01[i * as0] * local_beta1; });
        }
      }
      return 0;
    }
  };
} // namespace Tines

#endif
