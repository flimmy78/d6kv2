create or replace view subareaterminalview
(�ն����, �ն˴���, �ն�����, վ����ϵ����, վ����ϵ����, id, name, masterid)
as
select �ն˲�����.�ն����,
�ն˲�����.����,
�ն˲�����.����,
վ����ϵ������.����,
վ����ϵ������.����,
stationarea.id,
stationarea.NAME,
stationarea.MASTERID
from վ����ϵ������,�ն˲�����,stationarea
where �ն˲�����.վ����ϵ���� = վ����ϵ������.���� and (վ����ϵ������.������� = stationarea.id or վ����ϵ������.������� = 0)
order by stationarea.id;