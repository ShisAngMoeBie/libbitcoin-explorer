/*
 * Copyright (c) 2011-2014 sx developers (see AUTHORS)
 *
 * This file is part of sx.
 *
 * sx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "precompile.hpp"
#include <sx/command/stealth-address-encode.hpp>

#include <iostream>
#include <bitcoin/bitcoin.hpp>
#include <sx/define.hpp>
#include <sx/serializer/stealth.hpp>

using namespace bc;
using namespace sx;
using namespace sx::extension;
using namespace sx::serializer;

console_result stealth_address_encode::invoke(std::ostream& output,
    std::ostream& error)
{
    // Bound parameters.
    const stealth_prefix& prefix = get_prefix_option();
    const auto& scan_pubkey = get_scan_pubkey_argument();
    const auto& spend_pubkeys = get_spend_pubkeys_argument();
    const auto& signatures = get_signatures_option();
    const auto testnet = get_general_testnet_setting();

    // The implementation is safe for any value, but provide user feedback here.
    const auto max = if_else(spend_pubkeys.empty(), 1, spend_pubkeys.size());
    if (signatures > max)
    {
        error << SX_STEALTH_ADDRESS_ENCODE_SIGNATURES_OVERFLOW << std::endl;
        return console_result::failure;
    }

    // Issue a warning but don't prevent experimentation.
    if (spend_pubkeys.size() > 1)
        error << SX_STEALTH_ADDRESS_ENCODE_MULTISIG_NOT_SUPPORTED << std::endl;

    // Issue a warning but don't prevent experimentation.
    if (prefix.size() > 0)
        error << SX_STEALTH_ADDRESS_ENCODE_PREFIX_NOT_SUPPORTED << std::endl;

    // TESTNET WORKS WITHOUT RECOMPILE
    stealth address(prefix, scan_pubkey, spend_pubkeys, signatures, testnet);

    output << address << std::endl;
    return console_result::okay;
}