using GrpcPricing.Protos;
using MarketData;
using ParameterInfo;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HedgingEngine.GrpcClient
{
    public class PricerClient
    {
        GrpcPricingClient Client { get; set; }
        public TestParameters testParameters { get; set; }
        public double price { get; set; }
        public double priceStdDev { get; set; }
        public double[] deltas { get; set; }
        public double[] deltasStdDev { get; set; }

        public PricerClient(TestParameters testParameter, string serverAddress = "http://localhost:50051") {
            
            testParameters = testParameter;
            Client = new GrpcPricingClient(serverAddress);
            price = 0;
            priceStdDev = 0;
        }
        
        
        public void ComputePricesAndDeltas(DataFeed[] past, bool monitoringDateReached, double time)
        {
            PricingInput input = fillPricingInput(past, monitoringDateReached, time);
            PricingOutput output = Client.GrpcClient.PriceAndDeltas(input);
            price = output.Price;
            priceStdDev = output.PriceStdDev;
            deltas = output.Deltas.ToArray();
            deltasStdDev = output.DeltasStdDev.ToArray();
          
        }


        public ReqInfo ComputeHeartbeat()
        {
            ReqInfo reqInfo = Client.GrpcClient.Heartbeat(new Empty());
            return reqInfo;

        }



        private static PricingInput fillPricingInput(DataFeed[] past, bool monitoringDateReached, double time)
        {
            PricingInput input = new PricingInput();

            foreach (DataFeed dataFeed in past)
            {
                if (testParameters.PayoffDescription.PaymentDates.Contains(dataFeed.Date)) 
                {
                    PastLines pastLine = new PastLines();

                    foreach (double prix in dataFeed.SpotList.Values.ToArray())
                    {
                        pastLine.Value.Add(prix);
                    }

                    input.Past.Add(pastLine);
                }
            }
            input.Time = time;
            input.MonitoringDateReached = monitoringDateReached;
            return input;
        }

        public List<DataFeed> GetPast()
        {

        }





    }
}
