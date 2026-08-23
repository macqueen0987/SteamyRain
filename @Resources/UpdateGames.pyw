import os
import re
import subprocess
import sys
import configparser

script_dir = os.path.dirname(os.path.abspath(__file__))
if script_dir not in sys.path:
    sys.path.insert(0, script_dir)

from xbox_scan import discover_xbox_roots, scan_xbox_libraries

PLACEHOLDER_IMAGE = os.path.join(script_dir, "img", "placeholder_game.jpg")

class CaseSensitiveConfigParser(configparser.ConfigParser):
    def __init__(self, *args, **kwargs):
        super().__init__(
            delimiters=('='),
            comment_prefixes=(';'),
            inline_comment_prefixes=(),
            interpolation=None,
            strict=False,
            allow_no_value=True,
            *args, **kwargs
        )

    def optionxform(self, optionstr: str) -> str:
        return optionstr


extra_games_vars = {}
#__________________________________________________________________________________________________________________________#
#-------------------------------------------Function to update Update skin window------------------------------------------#

def update_rainmeter_status(status_message):
    subprocess.call([RainmeterPath, '!SetVariable', 'Status', fr'{status_message}', fr'{skinPath}Update'])
    subprocess.call([RainmeterPath, '!UpdateMeter', 'Status', fr'{skinPath}Update'])
#__________________________________________________________________________________________________________________________#
#----------------------------------------------Function to retrieve variables----------------------------------------------#

def get_variables(config_file):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    config_path = os.path.join(script_dir, config_file)
    config = CaseSensitiveConfigParser()
    config.read(config_path, encoding='utf-8')
    variables = {}
    if 'Variables' in config:
        # Normalize keys to lowercase for stable lookups (SkinInfo keeps mixed case)
        variables = {key.lower(): value for key, value in config['Variables'].items()}
    return variables

#__________________________________________________________________________________________________________________________#
#-------------------------------------Function to retrieve data from appmanifest files-------------------------------------#

def process_appmanifest_files(appmanifest_files, gamedir_path):
    processed_ids = []
    games_info = []

    for appmanifest_file in appmanifest_files:
        status = f"Processing file {appmanifest_file}"
        update_rainmeter_status(status)
        app_id = appmanifest_file[12:-4]  # Extract numerical part of the file name
        app_ids_to_skip = {'228980'}
        game_names_to_skip = {
            'Linux Runtime',
            'Proton'
        }

        if not app_id.isdigit():
            continue

        if app_id in app_ids_to_skip:
            continue

        # Read appmanifest file to get game information
        appmanifest_path = os.path.join(gamedir_path, appmanifest_file)
        with open(appmanifest_path, 'r', encoding='utf-8') as manifest_file:
            manifest_data = manifest_file.read()

            game_name = re.search(r'"name"\s*"(.*?)"', manifest_data)
            if not game_name:
                continue
            game_name = game_name.group(1)
            game_name = re.sub(r'[^a-zA-Z0-9\s]+', '', game_name)

            skip_game = False
            for game_to_skip in game_names_to_skip:
                if game_name.find(game_to_skip) != -1:
                    skip_game = True
                    break
            if skip_game:
                continue

            processed_ids.append(app_id)
            games_info.append({'appid': app_id, 'name': game_name, 'image': ""})

    return processed_ids, games_info

def normalize_stable_id(raw: str) -> str:
    value = str(raw).strip().strip('"')
    if value.isdigit():
        return f"steam:{value}"
    return value


def steam_launch(appid: str) -> str:
    return f"[steam://rungameid/{appid}]"


def scan_steam_libraries(steamapps_dirs, library_cache, status_fn):
    records = []
    for game_dir in steamapps_dirs:
        status_fn(f"Processing files of {game_dir}")
        if not os.path.isdir(game_dir):
            status_fn(f"Missing library: {game_dir}")
            continue
        appmanifest_files = [f for f in os.listdir(game_dir) if f.startswith("appmanifest_")]
        processed_ids, games_info = process_appmanifest_files(appmanifest_files, game_dir)
        for app_id, info in zip(processed_ids, games_info):
            records.append({
                "stable_id": f"steam:{app_id}",
                "name": info["name"],
                "launch": steam_launch(app_id),
                "image_path": "",
            })
    return records
#__________________________________________________________________________________________________________________________#
#------------------------------------------------Function to create meters-------------------------------------------------#
def create_meter(id_key, index, image, search, is_hidden, is_extra=False, extra_index=None, launch=None, image_name=None):
  
    section_prefix = 'E' if is_extra else ''
    HiddenValue = ('1' if search else
                    f'(1-#Vis{index}#)' if is_hidden and not is_extra else
                    f'(1-#{id_key}Vis#)' if is_hidden and is_extra else
                    f'#Vis{index}#' if not is_hidden and not is_extra else
                    f'#{id_key}Vis#')

    meter_data = {
        'Name': {
            'Meter': 'String',
            'Text': f'#{id_key}name#' if not is_extra else f'#{id_key}#',
            'LeftMouseUpAction': launch if not is_extra else f'[#{id_key}Path#]',
            'MeterStyle': 'HiddenNameStyle' if is_hidden else 'NameStyle',
            'Hidden': f'{HiddenValue}',
            'Group': f'Games | {section_prefix}G{index}',
        },
        'Image': {
            'Meter': 'Image',
            'MeterStyle': 'GameStyle',
            'ImageName': image_name if not is_extra else f'#@#img\\E{image}\\{extra_index:03d}.jpg',
            'LeftMouseUpAction': launch if not is_extra else f'[#{id_key}Path#]',
            'Hidden': f'{HiddenValue}',
            'Group': f'Games | {section_prefix}G{index}',
        },
        'String': {
            'Meter': 'String',
            'MeterStyle': 'VisStyle',
            'LeftMouseUpAction': f'[!WriteKeyValue Variables "Vis{index}" {"0" if is_hidden else "1"} "#@#GamesInfo.inc"]'
                                 f'[!WriteKeyValue Variables GameCountPLUS "(Clamp((#GameCountPLUS#{"+" if is_hidden else "-"}1),0,#GameCount#))" "#@#GamesInfo.inc"]'
                                 f'[!SetVariable GameCountPLUS "(Clamp((#GameCountPLUS#{"+" if is_hidden else "-"}1),0,#GameCount#))"]'
                                 f'[!SetVariable Vis{index} {"0" if is_hidden else "1"}]'
                                 f'[!UpdateMeasure HideGame][!Updatemeasure Lenght][!HideMeterGroup {section_prefix}G{index}][!UpdateMeterGroup Games][!ReDraw]'
                                 f'[!SetVariable UpdateVar "Vis{index}"][!SetVariable UpdateVar2 "GameCountPLUS"][!SetVariable UpdateVar3 "G{index}"][!Updatemeasure HiddenWindow]'
                                 if not is_extra else f'[!WriteKeyValue Variables "{id_key}Vis" {"0" if is_hidden else "1"} "#@#NonSteamGames.inc"]'
                                 f'[!WriteKeyValue Variables ExtraGameCountPLUS "(Clamp((#ExtraGameCountPLUS#{"+" if is_hidden else "-"}1),0,#ExtraGamesCount#))" "#@#NonSteamGames.inc"]'
                                 f'[!SetVariable ExtraGameCountPLUS "(Clamp((#ExtraGameCountPLUS#{"+" if is_hidden else "-"}1),0,#ExtraGamesCount#))"]'
                                 f'[!SetVariable {id_key}Vis {"0" if is_hidden else "1"}]'
                                 f'[!UpdateMeasure HideGame][!Updatemeasure Lenght][!HideMeterGroup {section_prefix}G{index}][!UpdateMeterGroup Games][!ReDraw]'
                                 f'[!SetVariable UpdateVar "{id_key}Vis"][!SetVariable UpdateVar2 "ExtraGameCountPLUS"][!SetVariable UpdateVar3 "EG{index}"][!Updatemeasure HiddenWindow]',
            'Hidden': f'{HiddenValue}',
            'Group': f'Games | {section_prefix}G{index}',
        },
        'Gap': {
            'Meter': 'Image',
            'MeterStyle': 'GapStyle',
        }
    }
    return meter_data
#__________________________________________________________________________________________________________________________#
#--------------------------------------------Function to check image existance---------------------------------------------#
def get_image_for_game(image_path, appid: str):
    app_image_dir = os.path.join(image_path, appid)
    if not os.path.isdir(app_image_dir):
        return "header.jpg"

    tail = ["header",
            "library_header",
            f"library_header_{locale}",
            "library_header_blur",
            "library_hero",
            "library_hero_blur",
            "logo",
            "library_600x900"
    ]
    image_types = ("jpg", "png")

    items = os.listdir(app_image_dir)
    # looking for tail + imagetype per folder
    for t in tail:
        for img_type in image_types:
            looking_for_file = f'{t}.{img_type}'

            if os.path.exists(os.path.join(app_image_dir, looking_for_file)):
                return looking_for_file

            for item in items:
                if os.path.isdir(os.path.join(app_image_dir, item)):
                    if os.path.exists(os.path.join(app_image_dir, item, looking_for_file)):
                        return f"{item}/{looking_for_file}"

    # take any other image you can find
    for item in items:
        if item.lower().endswith(('.jpg', '.png')):
            return item
        if os.path.isdir(os.path.join(app_image_dir, item)):
            for nested in os.listdir(os.path.join(app_image_dir, item)):
                if nested.lower().endswith(('.jpg', '.png')):
                    return f"{item}/{nested}"

    return "header.jpg"


def resolve_steam_image(library_cache: str, appid: str) -> str:
    filename = get_image_for_game(library_cache, appid)
    app_image_dir = os.path.join(library_cache, appid)
    if not os.path.isdir(app_image_dir):
        return ""
    image_path = os.path.join(app_image_dir, filename.replace('/', os.sep))
    if os.path.exists(image_path):
        return image_path
    return ""


def fill_steam_image_paths(records, library_cache) -> None:
    for record in records:
        if not record["stable_id"].startswith("steam:"):
            continue
        appid = record["stable_id"].split(":", 1)[1]
        image_path = resolve_steam_image(library_cache, appid)
        record["image_path"] = image_path or PLACEHOLDER_IMAGE


def merge_game_records(steam_records, xbox_records):
    return list(steam_records) + list(xbox_records)

#__________________________________________________________________________________________________________________________#
#--------------------------------------Extra slots: skip empty names at scan time------------------------------------------#

def iter_extra_game_indices(extra_vars: dict, extra_games_count: int):
    """Yield 1..extra_games_count indices that have a non-empty EgameN name.

    ExtraGamesCount remains the max used index (UI responsibility). Empty middle
    slots are skipped when writing meters so gaps do not produce blank tiles.
    """
    for i in range(1, extra_games_count + 1):
        name = extra_vars.get(f"Egame{i}", "")
        if isinstance(name, str):
            name = name.strip().strip('"')
        if name:
            yield i

#__________________________________________________________________________________________________________________________#
#------------------------------------------------Function to write meters--------------------------------------------------#

def write_meters(output, image, search, hidden_games, records=None):
    output_folder = 'dynamicMeters'
    subfolder_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), output_folder)
    os.makedirs(subfolder_path, exist_ok=True)
    config_combined = CaseSensitiveConfigParser()

    # Loop through each regular game record and create meters
    for i, record in enumerate(records or [], 1):
        id_key = f'ID{i}'
        is_hidden = hidden_games
        image_name = record["image_path"] or PLACEHOLDER_IMAGE
        if image != "Logo":
            image_name = os.path.join(os.path.dirname(image_name), "icon.jpg")
        meter_data = create_meter(
            id_key, i, image, search, is_hidden,
            launch=record["launch"],
            image_name=image_name,
        )
        if search or hidden_games:
            config_combined[f'Name{i}'] = meter_data['Name']
        config_combined[f'Game{i}'] = meter_data['Image']
        if not search:
            config_combined[f'Vis{i}'] = meter_data['String']
        config_combined[f'Gap{i}'] = meter_data['Gap']

    # Extra: only non-empty EgameN names (empty middle slots skipped; index gaps OK)
    for i in iter_extra_game_indices(extra_games_vars, extra_games_count):
        id_key = f'Egame{i}'
        is_hidden = hidden_games
        meter_data = create_meter(id_key, i, image, search, is_hidden, is_extra=True, extra_index=i)
        if search or hidden_games:
            config_combined[f'EName{i}'] = meter_data['Name']
        config_combined[f'EGame{i}'] = meter_data['Image']
        if not search:
            config_combined[f'EVis{i}'] = meter_data['String']
        config_combined[f'EGap{i}'] = meter_data['Gap']

    # Write meters to a single file
    output_name = 'dynamicSearchMeters' if search else 'dynamicMeters' if output == 1 else ('dynamicListMeters' if output == 2 and not hidden_games else 'dynamicHiddenMeters')
    output_file = os.path.join(subfolder_path, f'{output_name}.inc')
    with open(output_file, 'w', encoding='utf-8') as configfile:
        for section in config_combined.sections():
            configfile.write(f'[{section}]\n')
            for option, value in config_combined.items(section):
                configfile.write(f'{option}={value}\n')
            configfile.write('\n')
#__________________________________________________________________________________________________________________________#
#--------------------------------------------Function To update GamesInfo.inc----------------------------------------------#

def write_game_info(records):
    GamesInfoFile = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'GamesInfo.inc')
    existing_hidden_ids = set()

    if os.path.exists(GamesInfoFile):
        config_games_info = CaseSensitiveConfigParser()
        config_games_info.read(GamesInfoFile, encoding='utf-8')
        if 'Variables' in config_games_info:
            vars_section = config_games_info['Variables']
            for key, value in vars_section.items():
                if key.startswith('Vis') and value == '1':
                    index = key[3:]
                    id_key = f'ID{index}'
                    if id_key in vars_section:
                        existing_hidden_ids.add(normalize_stable_id(str(vars_section[id_key])))

    with open(GamesInfoFile, 'w', encoding='utf-8') as combined_file:
        combined_file.write('[Variables]\n')
        combined_file.write(f'GameCount={len(records)}\n')
        visible_count = sum(
            1 for record in records
            if normalize_stable_id(record['stable_id']) not in existing_hidden_ids
        )
        combined_file.write(f'GameCountPLUS={visible_count}\n')

        for index, record in enumerate(records, start=1):
            stable_id = record['stable_id']
            combined_file.write(f'ID{index}={stable_id}\n')
            game_name = record.get('name', '')
            game_name = ''.join(e for e in game_name if e.isalnum() or e.isspace())
            combined_file.write(f'ID{index}name="{game_name}"\n')
            hidden_value = 1 if normalize_stable_id(stable_id) in existing_hidden_ids else 0
            combined_file.write(f'Vis{index}={hidden_value}\n')
#__________________________________________________________________________________________________________________________#
#----------------------------------------------------SCRIPT START HERE-----------------------------------------------------#

def main():
    global RainmeterPath, skinPath, locale, processed_ids, game_count, extra_games_count, extra_games_vars

    # 1: Set Variables
    variables = get_variables('SkinInfo.inc')
    steam_path = variables.get('steampath', '')
    image_path = steam_path + '/appcache/librarycache'
    game_dirs = variables.get('gamedirs', '')
    game_dirs = game_dirs.split(',')
    for i in range(len(game_dirs)):
        game_dirs[i] = game_dirs[i].strip() + '\\steamapps'
    locale = variables.get('locale', '').lower()
    RainmeterPath = variables.get('rainmeterexe', '')
    skinMode = variables.get('mode')
    skinPath = 'SteamyRain\\'

    config_file = fr'SteamyRain.ini' if skinMode == '1' else fr'SteamyRainList.ini'

    subprocess.call([RainmeterPath, '!ActivateConfig', fr'{skinPath}Update'])
    subprocess.call([RainmeterPath, '!DeactivateConfig', fr'{skinPath}', fr'{config_file}'])

    # 2: Find installed game IDs and Names from appmanifest files
    status = "Processing appmanifest files..."
    update_rainmeter_status(status)
    steam_records = scan_steam_libraries(game_dirs, image_path, update_rainmeter_status)
    fill_steam_image_paths(steam_records, image_path)

    xboxdirs_raw = variables.get('xboxdirs', '')
    user_dirs = [d.strip() for d in xboxdirs_raw.split(',') if d.strip()]
    roots = discover_xbox_roots(user_dirs)
    if not roots:
        update_rainmeter_status("Xbox: no libraries found")
    xbox_records = scan_xbox_libraries(roots, PLACEHOLDER_IMAGE, update_rainmeter_status)
    if roots and not xbox_records:
        update_rainmeter_status("Xbox: no games found")
    records = merge_game_records(steam_records, xbox_records)

    # 3: Write new GamesInfo.inc
    status = "Writing new GamesInfo.inc..."
    update_rainmeter_status(status)
    write_game_info(records)

    # 4: Set variables for meters creation
    # ExtraGamesCount = max used index (UI). Empty middle EgameN names skipped in write_meters.
    game_count = len(records)
    processed_ids = [r["stable_id"].split(":", 1)[1] for r in records if r["stable_id"].startswith("steam:")]
    config_extra_games = CaseSensitiveConfigParser()
    config_extra_games.read(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'NonSteamGames.inc'), encoding='utf-8')
    extra_games_count = int(config_extra_games.get('Variables', 'ExtraGamesCount', fallback='0'))
    extra_games_vars = dict(config_extra_games['Variables']) if 'Variables' in config_extra_games else {}

    # 5: Create meters dynamically
    status = "Creating Dynamic Meters Files..."
    update_rainmeter_status(status)

    if game_count > 0 or extra_games_count > 0:
        output = 1
        image = 'Logo'
        hidden_games = False
        search = False
        write_meters(output, image, search, hidden_games, records=records)

        output = 2
        hidden_games = False
        write_meters(output, image, search, hidden_games, records=records)

        image = 'Icon'
        hidden_games = True
        write_meters(output, image, search, hidden_games, records=records)

        search = True
        write_meters(output, image, search, hidden_games, records=records)
    else:
        # No games found, create empty output files
        subfolder_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'dynamicMeters')
        os.makedirs(subfolder_path, exist_ok=True)
        output_file1 = os.path.join(subfolder_path, 'dynamicMeters.inc')
        output_file2 = os.path.join(subfolder_path, 'dynamicListMeters.inc')
        output_file3 = os.path.join(subfolder_path, 'dynamicHiddenMeters.inc')
        output_file4 = os.path.join(subfolder_path, 'dynamicSearchMeters.inc')
        open(output_file1, 'w', encoding='utf-8').close()
        open(output_file2, 'w', encoding='utf-8').close()
        open(output_file3, 'w', encoding='utf-8').close()
        open(output_file4, 'w', encoding='utf-8').close()

    subprocess.call([RainmeterPath, '!SetVariable', 'Loop', '3', fr'{skinPath}Update'])


if __name__ == '__main__':
    main()
