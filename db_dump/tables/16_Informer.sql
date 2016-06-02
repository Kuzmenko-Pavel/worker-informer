CREATE TABLE IF NOT EXISTS Informer
(
id INT8 PRIMARY KEY,
guid VARCHAR(64),
title VARCHAR(100),
account VARCHAR(2048),
domain VARCHAR(2048),
teasersCss TEXT,
headerHtml TEXT default '',
footerHtml TEXT default '',
nonrelevant VARCHAR(64),
user_code VARCHAR(2048),
auto_reload SMALLINT,
blinking SMALLINT,
shake SMALLINT,
blinking_reload SMALLINT,
shake_reload SMALLINT,
shake_mouse SMALLINT,
capacity SMALLINT,
valid SMALLINT,
html_notification SMALLINT,
place_branch SMALLINT,
retargeting_branch SMALLINT,
social_branch SMALLINT,
height SMALLINT,
width SMALLINT,
height_banner SMALLINT,
width_banner SMALLINT,
range_short_term REAL,
range_long_term REAL,
range_context REAL,
range_search REAL,
retargeting_capacity SMALLINT,
rating_division INT8 default 1000,
UNIQUE (id) ON CONFLICT IGNORE,
UNIQUE (guid) ON CONFLICT IGNORE
) WITHOUT ROWID;
CREATE INDEX IF NOT EXISTS idx_Informer_guid ON Informer (guid);
