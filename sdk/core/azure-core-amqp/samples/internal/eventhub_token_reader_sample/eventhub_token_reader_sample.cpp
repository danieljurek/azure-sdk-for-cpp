// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-Licence-Identifier: MIT

#undef _CRT_SECURE_NO_WARNINGS
#include <get_env.hpp>

#include <azure/core/amqp/connection.hpp>
#include <azure/core/amqp/message_receiver.hpp>
#include <azure/core/amqp/network/sasl_transport.hpp>

#include <chrono>
#include <iostream>
#include <limits>
#include <string>

int main()
{
  std::string eventhubConnectionString = GetEnvHelper::GetEnv("EVENTHUB_CONNECTION_STRING");

  auto credential
      = std::make_shared<Azure::Core::Amqp::_internal::ServiceBusSasConnectionStringCredential>(
          eventhubConnectionString);
  std::string entityPath = credential->GetEntityPath();
  if (entityPath.empty())
  {
    entityPath = GetEnvHelper::GetEnv("EVENTHUB_NAME");
  }

  Azure::Core::Amqp::_internal::ConnectionOptions connectionOptions;
  connectionOptions.ContainerId = "whatever";
  connectionOptions.EnableTrace = true;
  connectionOptions.Port = credential->GetPort();
  Azure::Core::Amqp::_internal::Connection connection(credential->GetHostName(), connectionOptions);

  Azure::Core::Amqp::_internal::SessionOptions sessionOptions;
  sessionOptions.InitialIncomingWindowSize = 100;
  Azure::Core::Amqp::_internal::Session session(connection, credential, sessionOptions);

  Azure::Core::Amqp::_internal::MessageReceiverOptions receiverOptions;
  receiverOptions.Name = "receiver-link";
  receiverOptions.MessageTarget = "ingress-rx";
  receiverOptions.SettleMode = Azure::Core::Amqp::_internal::ReceiverSettleMode::First;
  receiverOptions.MaxMessageSize = std::numeric_limits<uint16_t>::max();
  receiverOptions.EnableTrace = true;

  Azure::Core::Amqp::_internal::MessageReceiver receiver(
      session, entityPath + "/ConsumerGroups/$Default/Partitions/0", receiverOptions);
  // Open the connection to the remote.
  receiver.Open();

  auto timeStart = std::chrono::high_resolution_clock::now();

  constexpr int maxMessageReceiveCount = 1000;

  int messageReceiveCount = 0;
  while (messageReceiveCount < maxMessageReceiveCount)
  {
    auto message = receiver.WaitForIncomingMessage();
    std::cout << "Received message: " << message << std::endl;
    messageReceiveCount += 1;
  }

  auto timeEnd = std::chrono::high_resolution_clock::now();
  std::chrono::nanoseconds timeDiff = timeEnd - timeStart;

  std::cout << "Received " << messageReceiveCount << " in "
            << std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count()
            << " milliseconds ("
            << (static_cast<float>(messageReceiveCount)
                / static_cast<float>(
                    std::chrono::duration_cast<std::chrono::seconds>(timeDiff).count()))
            << " msg/sec)." << std::endl;

  receiver.Close();
}