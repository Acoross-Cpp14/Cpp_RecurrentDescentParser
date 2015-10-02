#pragma once

#define NO_COPY(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&);