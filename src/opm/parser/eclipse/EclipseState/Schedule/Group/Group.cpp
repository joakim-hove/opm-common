/*
  Copyright 2019 Equinor ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <utility>

#include <fmt/format.h>

#include <opm/parser/eclipse/EclipseState/Schedule/SummaryState.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/Group/Group.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQActive.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQConfig.hpp>
#include <opm/io/eclipse/rst/group.hpp>

#include "../eval_uda.hpp"

namespace Opm {

Group::Group()
    : Group("", 0, 0.0, UnitSystem())
{
}





Group::Group(const std::string& name, std::size_t insert_index_arg, double udq_undefined_arg, const UnitSystem& unit_system_arg) :
    m_name(name),
    m_insert_index(insert_index_arg),
    udq_undefined(udq_undefined_arg),
    unit_system(unit_system_arg),
    group_type(GroupType::NONE),
    gefac(1),
    transfer_gefac(true),
    vfp_table(0),
    production_properties(unit_system, name)
{
    // All groups are initially created as children of the "FIELD" group.
    if (name != "FIELD")
        this->parent_group = "FIELD";
}

Group::Group(const RestartIO::RstGroup& rst_group, std::size_t insert_index_arg, double udq_undefined_arg, const UnitSystem& unit_system_arg) :
    Group(rst_group.name, insert_index_arg, udq_undefined_arg, unit_system_arg)
{

    Group::GroupProductionProperties production(unit_system_arg, this->m_name);
    if (rst_group.oil_rate_limit != 0) {
        production.oil_target.update(rst_group.oil_rate_limit);
        production.production_controls += static_cast<int>(ProductionCMode::ORAT);
    }

    if (rst_group.gas_rate_limit != 0) {
        production.gas_target.update(rst_group.gas_rate_limit);
        production.production_controls += static_cast<int>(ProductionCMode::GRAT);
    }

    if (rst_group.water_rate_limit != 0) {
        production.water_target.update(rst_group.water_rate_limit);
        production.production_controls += static_cast<int>(ProductionCMode::WRAT);
    }

    if (rst_group.winj_cmode != 0) {
        Group::GroupInjectionProperties injection{this->m_name};
        injection.surface_max_rate.update(rst_group.water_surface_limit);
        injection.resv_max_rate.update(rst_group.water_reservoir_limit);
        injection.target_reinj_fraction.update(rst_group.water_reinject_limit);
        injection.target_void_fraction.update(rst_group.water_voidage_limit);
        injection.phase = Phase::WATER;
        injection.cmode = Group::InjectionCModeFromInt(rst_group.winj_cmode);
        this->updateInjection(injection);
    }

    if (rst_group.ginj_cmode != 0) {
        Group::GroupInjectionProperties injection{this->m_name};
        injection.surface_max_rate.update(rst_group.gas_surface_limit);
        injection.resv_max_rate.update(rst_group.gas_reservoir_limit);
        injection.target_reinj_fraction.update(rst_group.gas_reinject_limit);
        injection.target_void_fraction.update(rst_group.gas_voidage_limit);
        injection.phase = Phase::GAS;
        injection.cmode = Group::InjectionCModeFromInt(rst_group.ginj_cmode);
        this->updateInjection(injection);
    }

    if (rst_group.liquid_rate_limit != 0) {
        production.liquid_target.update(rst_group.liquid_rate_limit);
        production.production_controls += static_cast<int>(ProductionCMode::LRAT);
    }

    production.cmode = Group::ProductionCModeFromInt(rst_group.prod_cmode);

    production.guide_rate_def = Group::GuideRateProdTargetFromInt(rst_group.guide_rate_def);
    production.guide_rate = 0;
    if ((production.cmode == Group::ProductionCMode::ORAT) ||
        (production.cmode == Group::ProductionCMode::WRAT) ||
        (production.cmode == Group::ProductionCMode::GRAT) ||
        (production.cmode == Group::ProductionCMode::LRAT))
        production.exceed_action = Group::ExceedAction::RATE;

    if (production.production_controls != 0)
        this->updateProduction(production);
    Group::GroupInjectionProperties water_injection;
    water_injection.surface_max_rate.update(rst_group.water_surface_limit);
    water_injection.resv_max_rate.update(rst_group.water_reservoir_limit);
    water_injection.target_reinj_fraction.update(rst_group.water_reinject_limit);
    water_injection.target_void_fraction.update(rst_group.water_voidage_limit);
    water_injection.phase = Phase::WATER;
    water_injection.cmode = Group::InjectionCModeFromInt(rst_group.winj_cmode);

    if (rst_group.water_surface_limit != 0)
        water_injection.injection_controls += static_cast<int>(Group::InjectionCMode::RATE);

    if (rst_group.water_reservoir_limit != 0)
        water_injection.injection_controls += static_cast<int>(Group::InjectionCMode::RESV);

    if (rst_group.water_reinject_limit != 0)
        water_injection.injection_controls += static_cast<int>(Group::InjectionCMode::REIN);

    if (rst_group.water_voidage_limit != 0)
        water_injection.injection_controls += static_cast<int>(Group::InjectionCMode::VREP);

    if (water_injection.injection_controls != 0)
        this->updateInjection(water_injection);


    Group::GroupInjectionProperties gas_injection;
    gas_injection.surface_max_rate.update(rst_group.gas_surface_limit);
    gas_injection.resv_max_rate.update(rst_group.gas_reservoir_limit);
    gas_injection.target_reinj_fraction.update(rst_group.gas_reinject_limit);
    gas_injection.target_void_fraction.update(rst_group.gas_voidage_limit);
    gas_injection.phase = Phase::GAS;
    gas_injection.cmode = Group::InjectionCModeFromInt(rst_group.ginj_cmode);

    if (rst_group.gas_surface_limit != 0)
        gas_injection.injection_controls += static_cast<int>(Group::InjectionCMode::RATE);

    if (rst_group.gas_reservoir_limit != 0)
        gas_injection.injection_controls += static_cast<int>(Group::InjectionCMode::RESV);

    if (rst_group.gas_reinject_limit != 0)
        gas_injection.injection_controls += static_cast<int>(Group::InjectionCMode::REIN);

    if (rst_group.gas_voidage_limit != 0)
        gas_injection.injection_controls += static_cast<int>(Group::InjectionCMode::VREP);

    if (gas_injection.injection_controls != 0)
        this->updateInjection(gas_injection);
}


Group Group::serializeObject()
{
    Group result;
    result.m_name = "test1";
    result.m_insert_index = 1;
    result.udq_undefined = 3.0;
    result.unit_system = UnitSystem::serializeObject();
    result.group_type = GroupType::PRODUCTION;
    result.gefac = 4.0;
    result.transfer_gefac = true;
    result.vfp_table = 5;
    result.parent_group = "test2";
    result.m_wells = {{"test3", "test4"}, {"test5", "test6"}};
    result.m_groups = {{"test7", "test8"}, {"test9", "test10"}};
    result.injection_properties = {{Opm::Phase::OIL, GroupInjectionProperties::serializeObject()}};
    result.production_properties = GroupProductionProperties::serializeObject();
    result.m_topup_phase = Phase::OIL;
    result.m_gpmaint = GPMaint::serializeObject();

    return result;
}

std::size_t Group::insert_index() const {
    return this->m_insert_index;
}

const std::string& Group::name() const {
    return this->m_name;
}

bool Group::is_field() const {
    return (this->m_name == "FIELD");
}

const Group::GroupProductionProperties& Group::productionProperties() const {
    return this->production_properties;
}

const std::map<Phase, Group::GroupInjectionProperties>& Group::injectionProperties() const {
    return this->injection_properties;
}

int Group::getGroupNetVFPTable() const {
    return this->vfp_table;
}

bool Group::updateNetVFPTable(int vfp_arg) {
    if (this->vfp_table != vfp_arg) {
        this->vfp_table = vfp_arg;
        return true;
    } else
        return false;
}

namespace {
namespace detail {

bool has_control(int controls, Group::InjectionCMode cmode) {
    return ((controls & static_cast<int>(cmode)) != 0);
}

bool has_control(int controls, Group::ProductionCMode cmode) {
    return ((controls & static_cast<int>(cmode)) != 0);
}
}
}

bool Group::updateInjection(const GroupInjectionProperties& injection) {
    bool update = false;

    if (!this->hasType(GroupType::INJECTION)) {
        this->addType(GroupType::INJECTION);
        update = true;
    }

    auto iter = this->injection_properties.find(injection.phase);
    if (iter == this->injection_properties.end()) {
        this->injection_properties.insert(std::make_pair(injection.phase, injection));
        update = true;
    } else {
        if (iter->second != injection) {
            iter->second = injection;
            update = true;
        }
    }

    if (detail::has_control(injection.injection_controls, Group::InjectionCMode::RESV) ||
        detail::has_control(injection.injection_controls, Group::InjectionCMode::REIN) ||
        detail::has_control(injection.injection_controls, Group::InjectionCMode::VREP)) {
        auto topup_phase = injection.phase;
        if (topup_phase != this->m_topup_phase) {
            this->m_topup_phase = topup_phase;
            update = true;
        }
    } else {
        if (this->m_topup_phase.has_value())
            update = true;
        this->m_topup_phase = {};
    }
    return update;
}

bool Group::updateProduction(const GroupProductionProperties& production) {
    bool update = false;

    if (this->production_properties != production) {
        this->production_properties = production;
        update = true;
    }

    if (production.production_controls != 0) {
        if (!this->hasType(GroupType::PRODUCTION)) {
            this->addType(GroupType::PRODUCTION);
            update = true;
        }
    }

    return update;
}

Group::GroupInjectionProperties::GroupInjectionProperties(std::string group_name_arg)
    : GroupInjectionProperties(std::move(group_name_arg), Phase::WATER, UnitSystem(UnitSystem::UnitType::UNIT_TYPE_METRIC))
{}

Group::GroupInjectionProperties::GroupInjectionProperties(std::string group_name_arg,
                                                          const Phase phase_arg,
                                                          const UnitSystem& unit_system)
    : group_name { std::move(group_name_arg) }
    , phase { phase_arg }
    , surface_max_rate { unit_system.getDimension((phase == Phase::WATER)
                                                  ? UnitSystem::measure::liquid_surface_rate
                                                  : UnitSystem::measure::gas_surface_rate) }
    , resv_max_rate { unit_system.getDimension(UnitSystem::measure::rate) }
    , target_reinj_fraction { unit_system.getDimension(UnitSystem::measure::identity) }
    , target_void_fraction { unit_system.getDimension(UnitSystem::measure::identity) }
{}

Group::GroupInjectionProperties Group::GroupInjectionProperties::serializeObject()
{
    Group::GroupInjectionProperties result{"G"};
    result.phase = Phase::OIL;
    result.cmode = InjectionCMode::REIN;
    result.surface_max_rate = UDAValue(1.0);
    result.resv_max_rate = UDAValue(2.0);
    result.target_reinj_fraction = UDAValue(3.0);
    result.target_void_fraction = UDAValue(4.0);
    result.reinj_group = "test1";
    result.voidage_group = "test2";
    result.injection_controls = 5;
    result.guide_rate = 12345;
    result.guide_rate_def = Group::GuideRateInjTarget::NETV;

    return result;
}

bool Group::GroupInjectionProperties::operator==(const GroupInjectionProperties& other) const {
    return
        this->group_name              == other.group_name &&
        this->phase                   == other.phase &&
        this->cmode                   == other.cmode &&
        this->surface_max_rate        == other.surface_max_rate &&
        this->resv_max_rate           == other.resv_max_rate &&
        this->target_reinj_fraction   == other.target_reinj_fraction &&
        this->injection_controls      == other.injection_controls &&
        this->target_void_fraction    == other.target_void_fraction &&
        this->reinj_group             == other.reinj_group &&
        this->guide_rate              == other.guide_rate &&
        this->guide_rate_def          == other.guide_rate_def &&
        this->available_group_control == other.available_group_control &&
        this->voidage_group           == other.voidage_group;
}

bool Group::GroupInjectionProperties::operator!=(const GroupInjectionProperties& other) const {
    return !(*this == other);
}

bool Group::GroupInjectionProperties::updateUDQActive(const UDQConfig& udq_config, UDQActive& active) const {
    int update_count = 0;

    update_count += active.update(udq_config, this->surface_max_rate, this->group_name, UDAControl::GCONINJE_SURFACE_MAX_RATE);
    update_count += active.update(udq_config, this->resv_max_rate, this->group_name, UDAControl::GCONINJE_RESV_MAX_RATE);
    update_count += active.update(udq_config, this->target_reinj_fraction, this->group_name, UDAControl::GCONINJE_TARGET_REINJ_FRACTION);
    update_count += active.update(udq_config, this->target_void_fraction, this->group_name, UDAControl::GCONINJE_TARGET_VOID_FRACTION);

    return (update_count > 0);
}

Group::GroupProductionProperties::GroupProductionProperties() :
    GroupProductionProperties(UnitSystem(UnitSystem::UnitType::UNIT_TYPE_METRIC), "")
{}

Group::GroupProductionProperties::GroupProductionProperties(const UnitSystem& unit_system, const std::string& gname) :
    name(gname),
    oil_target(unit_system.getDimension(UnitSystem::measure::liquid_surface_rate)),
    water_target(unit_system.getDimension(UnitSystem::measure::liquid_surface_rate)),
    gas_target(unit_system.getDimension(UnitSystem::measure::gas_surface_rate)),
    liquid_target(unit_system.getDimension(UnitSystem::measure::liquid_surface_rate))
{
}

Group::GroupProductionProperties Group::GroupProductionProperties::serializeObject()
{
    Group::GroupProductionProperties result(UnitSystem(UnitSystem::UnitType::UNIT_TYPE_METRIC), "Group123");
    result.name = "Group123";
    result.cmode = ProductionCMode::PRBL;
    result.exceed_action = ExceedAction::WELL;
    result.oil_target = UDAValue(1.0);
    result.water_target = UDAValue(2.0);
    result.gas_target = UDAValue(3.0);
    result.liquid_target = UDAValue(4.0);
    result.guide_rate = 5.0;
    result.guide_rate_def = GuideRateProdTarget::COMB;
    result.resv_target = 6.0;
    result.production_controls = 7;

    return result;
}


bool Group::GroupProductionProperties::operator==(const GroupProductionProperties& other) const {
    return
        this->name                    == other.name &&
        this->cmode                   == other.cmode &&
        this->exceed_action           == other.exceed_action &&
        this->oil_target              == other.oil_target &&
        this->water_target            == other.water_target &&
        this->gas_target              == other.gas_target &&
        this->liquid_target           == other.liquid_target &&
        this->guide_rate              == other.guide_rate &&
        this->guide_rate_def          == other.guide_rate_def &&
        this->production_controls     == other.production_controls &&
        this->available_group_control == other.available_group_control &&
        this->resv_target             == other.resv_target;
}

bool Group::GroupProductionProperties::updateUDQActive(const UDQConfig& udq_config, UDQActive& active) const {
    int update_count = 0;

    update_count += active.update(udq_config, this->oil_target, this->name, UDAControl::GCONPROD_OIL_TARGET);
    update_count += active.update(udq_config, this->water_target, this->name, UDAControl::GCONPROD_WATER_TARGET);
    update_count += active.update(udq_config, this->gas_target, this->name, UDAControl::GCONPROD_GAS_TARGET);
    update_count += active.update(udq_config, this->liquid_target, this->name, UDAControl::GCONPROD_LIQUID_TARGET);

    return (update_count > 0);
}


bool Group::productionGroupControlAvailable() const {
    if (this->m_name == "FIELD")
        return false;
    return this->production_properties.available_group_control;
}

bool Group::injectionGroupControlAvailable(const Phase phase) const {
    if (this->m_name == "FIELD")
        return false;

    auto inj_iter = this->injection_properties.find(phase);
    if (inj_iter == this->injection_properties.end())
        return true;

    return inj_iter->second.available_group_control;
}

bool Group::GroupProductionProperties::operator!=(const GroupProductionProperties& other) const {
    return !(*this == other);
}



bool Group::hasType(GroupType gtype) const {
    return ((this->group_type & gtype) == gtype);
}

void Group::addType(GroupType new_gtype) {
    this->group_type = this->group_type | new_gtype;
}

const Group::GroupType& Group::getGroupType() const {
    return this-> group_type;
}

bool Group::isProductionGroup() const {
    return this->hasType(GroupType::PRODUCTION);
}

bool Group::isInjectionGroup() const {
    return this->hasType(GroupType::INJECTION);
}

void Group::setProductionGroup() {
    this->addType(GroupType::PRODUCTION);
}

void Group::setInjectionGroup() {
    this->addType(GroupType::INJECTION);
}


std::size_t Group::numWells() const {
    return this->m_wells.size();
}

const std::vector<std::string>& Group::wells() const {
    return this->m_wells.data();
}

const std::vector<std::string>& Group::groups() const {
    return this->m_groups.data();
}

bool Group::wellgroup() const {
    if (this->m_groups.size() > 0)
        return false;
    return true;
}

bool Group::addWell(const std::string& well_name) {
    if (!this->m_groups.empty())
        throw std::logic_error("Groups can not mix group and well children. Trying to add well: " + well_name + " to group: " + this->name());

    if (this->m_wells.count(well_name) == 0) {
        this->m_wells.insert(well_name);
        return true;
    }
    return false;
}

bool Group::hasWell(const std::string& well_name) const  {
    return (this->m_wells.count(well_name) == 1);
}

void Group::delWell(const std::string& well_name) {
    auto rm_count = this->m_wells.erase(well_name);
    if (rm_count == 0)
        throw std::invalid_argument("Group: " + this->name() + " does not have well: " + well_name);
}

bool Group::addGroup(const std::string& group_name) {
    if (!this->m_wells.empty())
        throw std::logic_error("Groups can not mix group and well children. Trying to add group: " + group_name + " to group: " + this->name());

    if (this->m_groups.count(group_name) == 0) {
        this->m_groups.insert(group_name);
        return true;
    }
    return false;
}

bool Group::hasGroup(const std::string& group_name) const  {
    return (this->m_groups.count(group_name) == 1);
}

void Group::delGroup(const std::string& group_name) {
    auto rm_count = this->m_groups.erase(group_name);
    if (rm_count == 0)
        throw std::invalid_argument("Group does not have group: " + group_name);
}

bool Group::update_gefac(double gf, bool transfer_gf) {
    bool update = false;
    if (this->gefac != gf) {
        this->gefac = gf;
        update = true;
    }

    if (this->transfer_gefac != transfer_gf) {
        this->transfer_gefac = transfer_gf;
        update = true;
    }

    return update;
}

double Group::getGroupEfficiencyFactor() const {
    return this->gefac;
}

bool Group::getTransferGroupEfficiencyFactor() const {
    return this->transfer_gefac;
}

const std::string& Group::parent() const {
    return this->parent_group;
}

std::optional<std::string> Group::control_group() const {
    if (m_name == "FIELD")
        return std::nullopt;
    else
        return this->parent();
}

std::optional<std::string> Group::flow_group() const {
    if (m_name == "FIELD")
        return std::nullopt;
    else
        return this->parent();
}

const std::optional<Phase>& Group::topup_phase() const {
    return this->m_topup_phase;
}



bool Group::updateParent(const std::string& parent) {
    if (this->parent_group != parent) {
        this->parent_group = parent;
        return true;
    }

    return false;
}

Group::ProductionControls Group::productionControls(const SummaryState& st) const {
    Group::ProductionControls pc;

    pc.cmode = this->production_properties.cmode;
    pc.exceed_action = this->production_properties.exceed_action;
    pc.oil_target = UDA::eval_group_uda(this->production_properties.oil_target, this->m_name, st, this->udq_undefined);
    pc.water_target = UDA::eval_group_uda(this->production_properties.water_target, this->m_name, st, this->udq_undefined);
    pc.gas_target = UDA::eval_group_uda(this->production_properties.gas_target, this->m_name, st, this->udq_undefined);
    pc.liquid_target = UDA::eval_group_uda(this->production_properties.liquid_target, this->m_name, st, this->udq_undefined);
    pc.guide_rate = this->production_properties.guide_rate;
    pc.guide_rate_def = this->production_properties.guide_rate_def;
    pc.resv_target = this->production_properties.resv_target;

    return pc;
}




Group::InjectionControls Group::injectionControls(Phase phase, const SummaryState& st) const {
    Group::InjectionControls ic;
    const auto& inj = this->injection_properties.at(phase);

    ic.phase = inj.phase;
    ic.cmode = inj.cmode;
    ic.injection_controls = inj.injection_controls;
    ic.surface_max_rate = UDA::eval_group_uda_rate(inj.surface_max_rate, this->m_name, st, this->udq_undefined, ic.phase, this->unit_system);
    ic.resv_max_rate = UDA::eval_group_uda(inj.resv_max_rate, this->m_name, st, this->udq_undefined);
    ic.target_reinj_fraction = UDA::eval_group_uda(inj.target_reinj_fraction, this->m_name, st, this->udq_undefined);
    ic.target_void_fraction = UDA::eval_group_uda(inj.target_void_fraction, this->m_name, st, this->udq_undefined);
    ic.reinj_group = inj.reinj_group.value_or(this->m_name);
    ic.voidage_group = inj.voidage_group.value_or(this->m_name);
    ic.guide_rate = inj.guide_rate;
    ic.guide_rate_def = inj.guide_rate_def;

    return ic;
}

bool Group::hasInjectionControl(Phase phase) const {
    return (this->injection_properties.count(phase) > 0);
}




Group::ProductionCMode Group::prod_cmode() const {
    return this->production_properties.cmode;
}

bool Group::ProductionControls::has_control(Group::ProductionCMode control) const {
    return detail::has_control(this->production_controls, control);
}

bool Group::InjectionControls::has_control(InjectionCMode cmode_arg) const {
    return detail::has_control(this->injection_controls, cmode_arg);
}

bool Group::has_control(Group::ProductionCMode control) const {
    return detail::has_control(production_properties.production_controls, control);
}

const std::optional<GPMaint>& Group::gpmaint() const {
    return this->m_gpmaint;
}

void Group::set_gpmaint(GPMaint gpmaint) {
    this->m_gpmaint = std::move(gpmaint);
}

void Group::set_gpmaint() {
    this->m_gpmaint = std::nullopt;
}


const std::string Group::ExceedAction2String( ExceedAction enumValue ) {
    switch(enumValue) {
    case ExceedAction::NONE:
        return "NONE";
    case ExceedAction::CON:
        return "CON";
    case ExceedAction::CON_PLUS:
        return "+CON";
    case ExceedAction::WELL:
        return "WELL";
    case ExceedAction::PLUG:
        return "PLUG";
    case ExceedAction::RATE:
        return "RATE";
    default:
        throw std::invalid_argument("unhandled enum value");
    }
}


Group::ExceedAction Group::ExceedActionFromString( const std::string& stringValue ) {

    if (stringValue == "NONE")
        return ExceedAction::NONE;
    else if (stringValue == "CON")
        return ExceedAction::CON;
    else if (stringValue == "+CON")
        return ExceedAction::CON_PLUS;
    else if (stringValue == "WELL")
        return ExceedAction::WELL;
    else if (stringValue == "PLUG")
        return ExceedAction::PLUG;
    else if (stringValue == "RATE")
        return ExceedAction::RATE;
    else
        throw std::invalid_argument("Unknown enum state string: " + stringValue );
}


const std::string Group::InjectionCMode2String( InjectionCMode enumValue ) {
    switch( enumValue ) {
    case InjectionCMode::NONE:
        return "NONE";
    case InjectionCMode::RATE:
        return "RATE";
    case InjectionCMode::RESV:
        return "RESV";
    case InjectionCMode::REIN:
        return "REIN";
    case InjectionCMode::VREP:
        return "VREP";
    case InjectionCMode::FLD:
        return "FLD";
    default:
        throw std::invalid_argument("Unhandled enum value");
    }
}


Group::InjectionCMode Group::InjectionCModeFromString( const std::string& stringValue ) {
    if (stringValue == "NONE")
        return InjectionCMode::NONE;
    else if (stringValue == "RATE")
        return InjectionCMode::RATE;
    else if (stringValue == "RESV")
        return InjectionCMode::RESV;
    else if (stringValue == "REIN")
        return InjectionCMode::REIN;
    else if (stringValue == "VREP")
        return InjectionCMode::VREP;
    else if (stringValue == "FLD")
        return InjectionCMode::FLD;
    else
        throw std::invalid_argument("Unknown enum state string: " + stringValue );
}

Group::GroupType operator|(Group::GroupType lhs, Group::GroupType rhs) {
    return static_cast<Group::GroupType>(static_cast<std::underlying_type<Group::GroupType>::type>(lhs) | static_cast<std::underlying_type<Group::GroupType>::type>(rhs));
}


Group::GroupType operator&(Group::GroupType lhs, Group::GroupType rhs) {
    return static_cast<Group::GroupType>(static_cast<std::underlying_type<Group::GroupType>::type>(lhs) & static_cast<std::underlying_type<Group::GroupType>::type>(rhs));
}


const std::string Group::ProductionCMode2String( ProductionCMode enumValue ) {
    switch( enumValue ) {
    case ProductionCMode::NONE:
        return "NONE";
    case ProductionCMode::ORAT:
        return "ORAT";
    case ProductionCMode::WRAT:
        return "WRAT";
    case ProductionCMode::GRAT:
        return "GRAT";
    case ProductionCMode::LRAT:
        return "LRAT";
    case ProductionCMode::CRAT:
        return "CRAT";
    case ProductionCMode::RESV:
        return "RESV";
    case ProductionCMode::PRBL:
        return "PRBL";
    case ProductionCMode::FLD:
        return "FLD";
    default:
        throw std::invalid_argument("Unhandled enum value");
    }
}


Group::ProductionCMode Group::ProductionCModeFromString( const std::string& stringValue ) {
    if (stringValue == "NONE")
        return ProductionCMode::NONE;
    else if (stringValue == "ORAT")
        return ProductionCMode::ORAT;
    else if (stringValue == "WRAT")
        return ProductionCMode::WRAT;
    else if (stringValue == "GRAT")
        return ProductionCMode::GRAT;
    else if (stringValue == "LRAT")
        return ProductionCMode::LRAT;
    else if (stringValue == "CRAT")
        return ProductionCMode::CRAT;
    else if (stringValue == "RESV")
        return ProductionCMode::RESV;
    else if (stringValue == "PRBL")
        return ProductionCMode::PRBL;
    else if (stringValue == "FLD")
        return ProductionCMode::FLD;
    else
        throw std::invalid_argument("Unknown enum state string: " + stringValue );
}

Group::ProductionCMode Group::ProductionCModeFromInt(int ecl_int) {
    switch (ecl_int) {
    case 0:
        // The inverse function returns 0 also for ProductionCMode::FLD.
        return ProductionCMode::NONE;
    case 1:
        return ProductionCMode::ORAT;
    case 2:
        return ProductionCMode::WRAT;
    case 3:
        return ProductionCMode::GRAT;
    case 4:
        return ProductionCMode::LRAT;
    case 5:
        return ProductionCMode::RESV;
    default:
        throw std::logic_error(fmt::format("Not recognized value: {} for PRODUCTION CMODE", ecl_int));
    }
}

int Group::ProductionCMode2Int(Group::ProductionCMode cmode) {
    switch (cmode) {
    case Group::ProductionCMode::NONE:
    case Group::ProductionCMode::FLD:
        // Observe that two production cmodes map to integer 0
        return 0;
    case Group::ProductionCMode::ORAT:
        return 1;
    case Group::ProductionCMode::WRAT:
        return 2;
    case Group::ProductionCMode::GRAT:
        return 3;
    case Group::ProductionCMode::LRAT:
        return 4;
    case Group::ProductionCMode::RESV:
        return 5;
    case Group::ProductionCMode::PRBL:
        return 6;
    case Group::ProductionCMode::CRAT:
        return 9;
    default:
        throw std::logic_error(fmt::format("Not handled enum value for Group Production CMODE"));
    }
}



Group::InjectionCMode Group::InjectionCModeFromInt(int ecl_int) {
    switch (ecl_int) {
    case 0:
        // The inverse function returns 0 also for InjectionCMode::FLD and InjectionCMode::SALE
        return InjectionCMode::NONE;
    case 1:
        return InjectionCMode::RATE;
    case 2:
        return InjectionCMode::RESV;
    case 3:
        return InjectionCMode::REIN;
    case 4:
        return InjectionCMode::VREP;
    default:
        throw std::logic_error(fmt::format("Not recognized value: {} for INJECTION CMODE", ecl_int));
    }
}

int Group::InjectionCMode2Int(InjectionCMode cmode) {
    switch (cmode) {
    case InjectionCMode::NONE:
    case InjectionCMode::FLD:
    case InjectionCMode::SALE:
        return 0;
    case InjectionCMode::RATE:
        return 1;
    case InjectionCMode::RESV:
        return 2;
    case InjectionCMode::REIN:
        return 3;
    case InjectionCMode::VREP:
        return 4;
    default:
        throw std::logic_error(fmt::format("Not handled enum value for Group Injection CMODE"));
    }
}

Group::GuideRateInjTarget Group::GuideRateInjTargetFromString( const std::string& stringValue ) {
    if (stringValue == "RATE")
        return GuideRateInjTarget::RATE;
    else if (stringValue == "RESV")
        return GuideRateInjTarget::RESV;
    else if (stringValue == "VOID")
        return GuideRateInjTarget::VOID;
    else if (stringValue == "NETV")
        return GuideRateInjTarget::NETV;
    else
        return GuideRateInjTarget::NO_GUIDE_RATE;
}

Group::GuideRateProdTarget Group::GuideRateProdTargetFromString( const std::string& stringValue ) {
    if (stringValue == "OIL")
        return GuideRateProdTarget::OIL;
    else if (stringValue == "WAT")
        return GuideRateProdTarget::WAT;
    else if (stringValue == "GAS")
        return GuideRateProdTarget::GAS;
    else if (stringValue == "LIQ")
        return GuideRateProdTarget::LIQ;
    else if (stringValue == "COMB")
        return GuideRateProdTarget::COMB;
    else if (stringValue == "WGA")
        return GuideRateProdTarget::WGA;
    else if (stringValue == "CVAL")
        return GuideRateProdTarget::CVAL;
    else if (stringValue == "INJV")
        return GuideRateProdTarget::INJV;
    else if (stringValue == "POTN")
        return GuideRateProdTarget::POTN;
    else if (stringValue == "FORM")
        return GuideRateProdTarget::FORM;
    else if (stringValue == " ")
        return GuideRateProdTarget::NO_GUIDE_RATE;
    else
        return GuideRateProdTarget::NO_GUIDE_RATE;
}


// Integer values defined vectoritems/group.hpp
Group::GuideRateProdTarget Group::GuideRateProdTargetFromInt(int ecl_id) {
    switch(ecl_id) {
    case 0:
        return GuideRateProdTarget::NO_GUIDE_RATE;
    case 1:
        return GuideRateProdTarget::OIL;
    case 2:
        return GuideRateProdTarget::WAT;
    case 3:
        return GuideRateProdTarget::GAS;
    case 4:
        return GuideRateProdTarget::LIQ;
    case 7:
        return GuideRateProdTarget::POTN;
    case 8:
        return GuideRateProdTarget::FORM;
    case 9:
        return GuideRateProdTarget::COMB;
    default:
        throw std::logic_error(fmt::format("Integer GuideRateProdTarget: {} not recognized", ecl_id));
    }
}

bool Group::operator==(const Group& data) const
{
    return this->name() == data.name() &&
           this->insert_index() == data.insert_index() &&
           this->udq_undefined == data.udq_undefined &&
           this->unit_system == data.unit_system &&
           this->group_type == data.group_type &&
           this->getGroupEfficiencyFactor() == data.getGroupEfficiencyFactor() &&
           this->getTransferGroupEfficiencyFactor() == data.getTransferGroupEfficiencyFactor() &&
           this->getGroupNetVFPTable() == data.getGroupNetVFPTable() &&
           this->parent() == data.parent() &&
           this->m_wells == data.m_wells &&
           this->m_groups == data.m_groups &&
           this->m_topup_phase == data.m_topup_phase &&
           this->injection_properties == data.injection_properties &&
           this->m_gpmaint == data.m_gpmaint &&
           this->productionProperties() == data.productionProperties();
}

}
