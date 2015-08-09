/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.
 */

#ifndef HUDSON_IMC_H
#define HUDSON_IMC_H

#include "Porting.h"
#include "AGESA.h"
#include <FchCommonCfg.h>

void imc_reg_init(void);
void enable_imc_thermal_zone(void);
void oem_fan_control(FCH_DATA_BLOCK *FchParams);

#endif
