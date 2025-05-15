#!/usr/bin/env python3
import sqlite3, json, sys, os

def dump_to_json(sqlite_path, json_path, table_name=None):
    if not os.path.exists(sqlite_path):
        print(f"Ошибка: не найден файл {sqlite_path}")
        return

    conn = sqlite3.connect(sqlite_path)
    cur  = conn.cursor()

    # Если имя таблицы не указано, берём первую «пользовательскую»
    if table_name is None:
        cur.execute("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';")
        tbls = [row[0] for row in cur.fetchall()]
        if not tbls:
            print("Ошибка: не найдено ни одной таблицы")
            return
        table = tbls[0]
    else:
        table = table_name

    # Получаем инфо по колонкам
    cur.execute(f"PRAGMA table_info({table});")
    cols = [row[1] for row in cur.fetchall()]  # row[1] = имя колонки

    # Пытаемся найти именно «Time» и «Value» (игнорируя регистр)
    lower = [c.lower() for c in cols]
    try:
        time_idx  = lower.index("time")
        value_idx = lower.index("value")
    except ValueError:
        # Если нет, просто берём первые две
        time_idx, value_idx = 0, 1

    # Читаем все строки
    cur.execute(f"SELECT * FROM {table};")
    rows = cur.fetchall()

    # Собираем массив массивов [time, value]
    out = []
    for r in rows:
        t = r[time_idx]
        v = r[value_idx]
        # SQLite может вернуть числа как int/float или строки,
        # поэтому приводим всё к строкам нужного формата:
        t_str = str(t)
        v_num = float(v) if isinstance(v, (int, float)) else float(str(v))
        out.append([t_str, v_num])

    # Записываем
    with open(json_path, "w", encoding="utf8") as f:
        json.dump(out, f, ensure_ascii=False, indent=2)

    print(f"Таблица `{table}` из `{sqlite_path}` → `{json_path}` готова.")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Использование: dump.py input.sqlite output.json [table_name]")
    else:
        dump_to_json(sys.argv[1], sys.argv[2],
                     sys.argv[3] if len(sys.argv) > 3 else None)
