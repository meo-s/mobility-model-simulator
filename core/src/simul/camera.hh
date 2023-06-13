#pragma once
#ifndef _SIMUL_CAMERA_HH
#define _SIMUL_CAMERA_HH

#include "fundamental/rect2.hh"
#include "fundamental/types.hh"

class Camera {
 public:
  using Precision = f64;

  Camera() noexcept = default;

  Camera(Precision hfov, Precision vfov) noexcept;

  Camera(Camera const&) noexcept = default;

  Camera& operator=(Camera const&) noexcept = default;

  Precision getHFOV() const noexcept;

  void setHFOV(Precision) noexcept;

  Precision getVFOV() const noexcept;

  void setVFOV(Precision) noexcept;

  void setFOV(Precision hfov, Precision vfov) noexcept;

  Rect2<Precision> getFilm(Precision distance = 1.f) const noexcept;

 private:
  Precision _tan_hfov;

  Precision _tan_vfov;
};

#endif
