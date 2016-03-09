SELECT 	inf.id,
    inf.title,
	inf.capacity,
	inf.bannersCss,
	inf.teasersCss,
    inf.headerHtml,
    inf.footerHtml,
	inf.domainId,
    dm.name,
	inf.accountId,
    ac.name,
    inf.range_short_term,
	inf.range_long_term,
	inf.range_context,
	inf.range_search,
	inf.retargeting_capacity,
	ac.blocked,
    inf.nonrelevant,
    inf.user_code,
    inf.html_notification,
    inf.plase_branch,
    inf.retargeting_branch
FROM Informer AS inf INDEXED BY idx_Informer_guid
INNER JOIN Accounts AS ac ON inf.accountId = ac.id
INNER JOIN Domains AS dm ON inf.domainId = dm.id
WHERE inf.guid='%s'
LIMIT 1;
