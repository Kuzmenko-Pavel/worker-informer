SELECT 	inf.id,
    inf.title,
	inf.capacity,
	inf.bannersCss,
	inf.teasersCss,
    inf.headerHtml,
    inf.footerHtml,
    inf.range_short_term,
	inf.range_long_term,
	inf.range_context,
	inf.range_search,
	inf.retargeting_capacity,
    inf.valid,
    inf.nonrelevant,
    inf.user_code,
    inf.html_notification,
    inf.place_branch,
    inf.retargeting_branch
FROM Informer AS inf INDEXED BY idx_Informer_guid
WHERE inf.guid='%s'
LIMIT 1;
