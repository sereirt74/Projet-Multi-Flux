using HedgingEngine.GrpcClient;
using HedgingEngine.RebalancingOracle;
using MarketData;
using ParameterInfo;
using ParameterInfo.RebalancingOracleDescriptions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TimeHandler;

namespace HedgingEngine.Hedger
{
    public class Hedger
    {
        public PricerClient PricerClient;
        public List<OutputData> OutputDataList;
        public Portfolio.Portfolio HedgingPortfolio;
        public IRebalacingOracle rebalacingOracle;

        public Hedger(DataFeed[] dataFeeds, PricerClient pricer)
        {
            PricerClient = pricer;
            OutputDataList = new List<OutputData>();
            InitializeRebalancingOracle();
            InitializePortfolio(dataFeeds);
        }


        private void InitializePortfolio(DataFeed[] past)
        {
            TestParameters testParams = PricerClient.testParameters;
            DateTime currentDate = past[past.Length - 1].Date;
            bool monitoringDateReached = testParams.PayoffDescription.PaymentDates.Contains(currentDate);
            MathDateConverter mathDateConverter = new MathDateConverter(testParams.NumberOfDaysInOneYear);
            double time = mathDateConverter.ConvertToMathDistance(testParams.PayoffDescription.CreationDate, currentDate);

            PricerClient.ComputePricesAndDeltas(past, monitoringDateReached, time);
            int nbAssets = PricerClient.deltas.Length;
            double initPrice = PricerClient.price;
            HedgingPortfolio = new Portfolio.Portfolio(initPrice, nbAssets);
            DataFeed initDataFeed = past[0];
            HedgingPortfolio.updateComposition(PricerClient.deltas.ToArray(), initDataFeed);
            fillOutputDataList(currentDate);

        }

        private void InitializeRebalancingOracle()
        {
            IRebalancingOracleDescription description = PricerClient.testParameters.RebalancingOracleDescription;
            rebalacingOracle = new FixedTimesOracle(description);
        }


        public void Hedge(List<DataFeed> dataFeeds)
        {
            TestParameters testParams = PricerClient.testParameters;
            MathDateConverter mathDateConverter = new MathDateConverter(testParams.NumberOfDaysInOneYear);
            double riskFreeRate = testParams.AssetDescription.CurrencyRates[testParams.AssetDescription.DomesticCurrencyId];
            DateTime OldDate = dataFeeds[0].Date;
            
            foreach (DataFeed dataFeed in dataFeeds.Skip(1))
            {
                DateTime currentDate = dataFeed.Date;
                double time = mathDateConverter.ConvertToMathDistance(OldDate, currentDate);
                bool monitoringDateReached = testParams.PayoffDescription.PaymentDates.Contains(currentDate);
                List<DataFeed> newPast = new List<DataFeed>(dataFeeds);
                newPast.Add(dataFeed);
                PricerClient.ComputePricesAndDeltas(newPast.ToArray(), monitoringDateReached, time);
                HedgingPortfolio.UpdatePortfolioValue(dataFeed, time, riskFreeRate);
                OldDate = currentDate;
                if (rebalacingOracle.rebalancingDate(currentDate))
                {
                    HedgingPortfolio.updateComposition(PricerClient.deltas, dataFeed);
                    fillOutputDataList(currentDate);
                }
            }
        }

        private void fillOutputDataList(DateTime date)
        {
            OutputDataList.Add(new OutputData()
            {
                Date = date,
                Deltas = PricerClient.deltas,
                DeltasStdDev = PricerClient.deltasStdDev,
                Price = PricerClient.price,
                PriceStdDev = PricerClient.priceStdDev,
                Value = HedgingPortfolio.PortfolioValue
            }) ;

        }
    }
}
