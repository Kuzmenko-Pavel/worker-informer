CREATE TABLE IF NOT EXISTS Informer
(
id INT8 PRIMARY KEY,
guid VARCHAR(64),
title VARCHAR(100),
teasersCss TEXT,
bannersCss TEXT,
headerHtml TEXT default '',
footerHtml TEXT default '',
domainId INTEGER,
accountId INTEGER,
nonrelevant VARCHAR(64),
user_code VARCHAR(2048),
capacity SMALLINT,
valid SMALLINT,
html_notification SMALLINT,
plase_branch SMALLINT,
retargeting_branch SMALLINT,
height SMALLINT,
width SMALLINT,
height_banner SMALLINT,
width_banner SMALLINT,
range_short_term REAL,
range_long_term REAL,
range_context REAL,
range_search REAL,
retargeting_capacity SMALLINT,
UNIQUE (id) ON CONFLICT IGNORE,
UNIQUE (guid) ON CONFLICT IGNORE,
FOREIGN KEY(domainId) REFERENCES Domains(id),
FOREIGN KEY(accountId) REFERENCES Accounts(id)
);
CREATE INDEX IF NOT EXISTS idx_Informer_guid ON Informer (guid);
CREATE INDEX IF NOT EXISTS idx_Informer_domainId ON Informer (domainId);
CREATE INDEX IF NOT EXISTS idx_Informer_accountId ON Accounts (id);
INSERT INTO Informer(id,guid,teasersCss,bannersCss,domainId,accountId,title) VALUES(1,'1','','',1,1,'');
