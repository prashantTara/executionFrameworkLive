awk '{ 
{split($(NF-7),pdays,":")}; 
{split($(NF-5),ndays,":")};
{split($(NF-3),zdays,":")}; 
{split($(NF-12),avg,":")}; 
{split($(NF-9),std,":")};
{split($(NF-8),ddown,":")};
{split($10,buy,":")};
{split($11,sell,":")};
print avg[2]/std[2],(avg[2]*(pdays[2]+ndays[2]+zdays[2]))/ddown[2],((pdays[2]+ndays[2]+zdays[2])*avg[2]+buy[2]+sell[2])/(buy[2]+sell[2]),pdays[2]/ndays[2],$1; }' COMBINED_ALL_STRATS.txt
