using HedgingEngine.GrpcClient;
using MarketData;
using ParameterInfo;
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
        public TestParameters testParameters;
        public PricerClient PricerClient;
        public List<double> Prices;
        public List<double> PricesStdDev;
        public List<double> Deltas;
        public List<double> DeltasStdDev;
        public List<DateTime> Dates;
        public List<double> PortfolioValues; 
        public DataFeed[] Past;
        public Portfolio.Portfolio HedgingPortfolio;





        public Hedger(DataFeed firstDataFeed, TestParameters testParameters, string serverAddress = "http://localhost:50051")
        {
            this.testParameters = testParameters;
            PricerClient = new PricerClient(testParameters, serverAddress);

            InitializePortfolio();
        }


        private void InitializePortfolio(List<DataFeed> past)
        {
            DateTime currentDate = past[past.Count - 1].Date;
            bool monitoringDateReached = testParameters.PayoffDescription.PaymentDates.Contains(currentDate);
            MathDateConverter mathDateConverter = new MathDateConverter(testParameters.NumberOfDaysInOneYear);
            double time = mathDateConverter.ConvertToMathDistance(testParameters.PayoffDescription.CreationDate, currentDate);
            
            PricerClient.ComputePricesAndDeltas(past.ToArray(), monitoringDateReached, time);
            int nbAssets = PricerClient.deltas.Length;
            HedgingPortfolio = new Portfolio.Portfolio(0, 0, nbAssets);


        }

       
        

        

        public void Hedge(DataFeed[] dataFeeds)
        {

            foreach (DataFeed dataFeed in dataFeeds)
            {
                MathDateConverter mathDateConverter = new MathDateConverter(testParameters.NumberOfDaysInOneYear);
                double riskFreeRate = mathDateConverter.ConvertToMathDistance(dataFeed.Date, testParameters.PayoffDescription.PaymentDates.ElementAtOrDefault(0));

                PricerClient.ComputePricesAndDeltas(Past, riskFreeRate, riskFreeRate);

        }
    }




}
